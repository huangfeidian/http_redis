#include <server/http_server.h>
#include <nlohmann/json.hpp>
#include <spdlog/fmt/ostr.h>
using namespace spiritsaway;


using namespace spiritsaway::http_redis;
using json = nlohmann::json;

redis_session::redis_session(tcp::socket&& socket,
	logger_t in_logger,
	std::uint32_t in_expire_time,
	concurrency::task_channels<task, true>& in_task_dest)
	: http_utils::server::session(std::move(socket), std::move(in_logger), in_expire_time)
	, _task_dest(in_task_dest)
{

}
std::string redis_session::check_request()
{
	auto check_error = http_utils::server::session::check_request();
	if (!check_error.empty())
	{
		return check_error;
	}
	if (!req_.target().starts_with("/redis/post/"))
	{
		return "request target must start with /redis/post/";
	}
	if (!json::accept(req_.body()))
	{
		logger->info("request is {}", req_);
		return "body must be json";
	}
	auto json_body = json::parse(req_.body());
	if (!json_body.is_object())
	{
		return "body must be json object";

	}
	std::vector<std::string> str_values;
	str_values.reserve(2);
	std::vector<std::string> keys = { "request_id", "channel" };
	for (const auto& one_key : keys)
	{
		auto cur_iter = json_body.find(one_key);
		if (cur_iter == json_body.end())
		{
			return "cant find value for " + one_key;
		}
		if (!cur_iter->is_string())
		{
			return "value for key " + one_key + " is not string";
		}
		str_values.push_back(cur_iter->get<std::string>());

	}
	request_id = str_values[0];
	channel = str_values[1];
	auto cmds_iter = json_body.find("redis_cmds");
	if (cmds_iter == json_body.end())
	{
		return "cant find value for key redis_cmds";
	}
	if (!cmds_iter->is_array())
	{
		return "value for key redis_cmds is not array";
	}
	for (const auto& one_value : *cmds_iter)
	{
		if (!one_value.is_string())
		{
			return "values in redis_cmd array must be string";
		}
		redis_cmds.push_back(one_value.get<std::string>());
	}
	return "";
}
void redis_session::route_request()
{
	logger->debug("redis_session accept new request {} body {}", request_id, req_.body());

	auto self = std::dynamic_pointer_cast<redis_session>(shared_from_this());
	auto cur_task_lambda = [=](const std::vector<reply>& replys)
	{
		return self->finish_task(replys);
	};
	callback = std::make_shared<task::callback_t>(cur_task_lambda);
	auto cur_task = std::make_shared<task>(channel, redis_cmds, request_id, callback);
	beast::get_lowest_layer(stream_).expires_after(
		std::chrono::seconds(expire_time));
	expire_timer = std::make_shared<boost::asio::steady_timer>(stream_.get_executor(), std::chrono::seconds(expire_time / 2));
	expire_timer->async_wait([self](const boost::system::error_code& e) {
		self->on_timeout(e);
	});
	_task_dest.add_task(cur_task);

}
void redis_session::on_timeout(const boost::system::error_code& e)
{
	if (e == boost::asio::error::operation_aborted)
	{
		return;
	}
	expire_timer.reset();
	callback.reset();

	do_write(http_utils::common::create_response::bad_request("timeout", req_));
}

void redis_session::finish_task(const std::vector<reply>& replys)
{
	expire_timer.reset();
	callback.reset();
	json::array_t array_result;
	for (const auto& one_reply : replys)
	{
		json one_result;
		if (!one_reply.error.empty())
		{
			one_result["error"] = one_reply.error;
			logger->info("process {} with error {}", req_.body(), one_reply.error);
		}
		else
		{
			if (one_reply.is_array)
			{
				one_result["content"] = json(one_reply.content);
				//for (auto one_str : one_reply.content)
				//{
				//	logger->info("content is {}", one_str);

				//}
			}
			else
			{
				if (!one_reply.content.empty())
				{
					one_result["content"] = one_reply.content[0];
					//logger->info("content is {}", one_reply.content[0]);

				}
				else
				{
					one_result["error"] = "empty content";
				}
			}
		}
		array_result.push_back(one_result);
	}
	json final_result = json(array_result);
	logger->debug("finish task {}", request_id);
	http::response<http::string_body> res{ http::status::ok, req_.version() };
	res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
	res.set(http::field::content_type, "text/html");
	res.keep_alive(req_.keep_alive());
	res.body() = final_result.dump();
	res.prepare_payload();
	do_write(std::move(res));
}

redis_listener::redis_listener(net::io_context& ioc,
	tcp::endpoint endpoint,
	logger_t in_logger,
	std::uint32_t expire_time,
	concurrency::task_channels<task, true>& task_dest)
	: http_utils::server::listener(ioc, std::move(endpoint), std::move(in_logger), expire_time)
	, _task_dest(task_dest)
{

}

std::shared_ptr<http_utils::server::session> redis_listener::make_session(tcp::socket&& socket)
{
	return std::make_shared<redis_session>(std::move(socket), logger, expire_time, _task_dest);
}