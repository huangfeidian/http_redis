#include "server/server.h"
#include <nlohmann/json.hpp>
#include <spdlog/fmt/ostr.h>
using namespace spiritsaway;


using namespace spiritsaway::http_redis;
using json = nlohmann::json;


std::string redis_server::check_request(const http_utils::request& cur_req, task_desc& cur_task_desc) const
{

	if (cur_req.uri.find("/redis/post/") != 0)
	{
		return "request target must start with /redis/post/";
	}
	if (!json::accept(cur_req.body))
	{
		m_logger->info("request is {}", cur_req.to_string("", ""));
		return "body must be json";
	}
	auto json_body = json::parse(cur_req.body);
	try
	{
		json_body.get_to(cur_task_desc);
	}
	catch (std::exception& e)
	{
		m_logger->info("cant parse task_desc with data {} error is {}", cur_req.body, e.what());
		return "task_desc parse fail";
	}
	return "";
}
void redis_server::handle_request(const http_utils::request& cur_req, http_utils::reply_handler rep_cb)
{
	
	task_desc cur_task_desc;
	std::string check_err = check_request(cur_req, cur_task_desc);
	if (!check_err.empty())
	{
		m_logger->debug("redis_session parse  body {} fail {}", cur_req.body, check_err);
		return;
	}
	auto cur_task_lambda = [=](const std::vector<reply>& replys)
	{
		finish_task(cur_req, replys, rep_cb);
	};
	
	auto cur_task = std::make_shared<task>(cur_task_desc, cur_task_lambda);

	m_task_dest.add_task(cur_task);

}


void redis_server::finish_task(const http_utils::request& cur_req, const std::vector<reply>& replys, http_utils::reply_handler rep_cb)
{

	json::array_t array_result;
	for (const auto& one_reply : replys)
	{
		json one_result;
		if (!one_reply.error.empty())
		{
			one_result["error"] = one_reply.error;
			m_logger->info("process {} with error {}", cur_req.body, one_reply.error);
		}
		else
		{
			if (one_reply.is_array)
			{
				one_result["content"] = json(one_reply.content);

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
	http_utils::reply cur_rep;
	cur_rep.status_code = std::uint32_t(http_utils::reply::status_type::ok);
	cur_rep.content = final_result.dump();
	cur_rep.add_header("Content-Type", "text/json" );

	rep_cb(cur_rep);
}

redis_server::redis_server(asio::io_context& ioc
	, std::shared_ptr<spdlog::logger> in_logger, const std::string& address, const std::string& port, concurrency::task_channels<task, true>& task_dest)
	: http_utils::http_server(ioc, in_logger, address, port)
	, m_task_dest(task_dest)
{

}
