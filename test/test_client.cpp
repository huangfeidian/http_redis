#include <command/hash.h>
#include <command/list.h>
#include <command/zset.h>
#include <command/key.h>
#include <command/set.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/ostr.h>
#include <http_utils/http_client.h>

#include <nlohmann/json.hpp>
using namespace spiritsaway;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;
using logger_t = std::shared_ptr<spdlog::logger>;

std::shared_ptr<spdlog::logger> create_logger(const std::string& name)
{
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::debug);
	std::string pattern = "[" + name + "] [%^%l%$] %v";
	console_sink->set_pattern(pattern);

	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(name + ".log", true);
	file_sink->set_level(spdlog::level::trace);
	auto logger = std::make_shared<spdlog::logger>(name, spdlog::sinks_init_list{ console_sink, file_sink });
	logger->set_level(spdlog::level::trace);
	return logger;
}

std::vector<std::string> create_hash_cmds()
{
	std::vector<std::string> result;
	return result;
}
std::vector<std::string> create_list_cmds()
{
	std::vector<std::string> result;
	return result;
}
std::vector<std::string> create_zset_cmds()
{
	std::vector<std::string> result;
	return result;
}
std::vector<std::string> create_string_cmds()
{
	std::vector<std::string> result;
	result.push_back(http_redis::command::key::set("a", "a_1", 100 * 1000));
	result.push_back(http_redis::command::key::set("a", "a_2", 100 * 1000, http_redis::command::set_mode::nx));
	result.push_back(http_redis::command::key::set("a", "a_3", 0, http_redis::command::set_mode::nx));
	std::unordered_map<std::string, std::string> data_1 = {
		{"a", "a_m"},
		{"b", "b_m"},
		{"c", "c_m"}
	};
	result.push_back(http_redis::command::key::set(data_1, true));
	result.push_back(http_redis::command::key::set(data_1, false));
	result.push_back(http_redis::command::key::get("a"));
	std::vector<std::string> keys_1;
	for (const auto&[k, v] : data_1)
	{
		keys_1.push_back(k);
	}
	result.push_back(http_redis::command::key::get(keys_1));
	result.push_back(http_redis::command::key::del(keys_1));



	return result;
}
std::vector<std::string> create_set_cmds()
{
	std::vector<std::string> result;
	return result;
}

void make_request(net::io_context& ioc, logger_t cur_logger,
	const std::string& host, const std::uint16_t& port, const std::uint32_t& expire_time, 
	std::uint32_t& next_cmd_idx, const std::vector<std::string>& cmds)
{
	if (next_cmd_idx >= cmds.size())
	{
		return;
	}
	http_utils::common::request_data cur_request;
	cur_request.host = "127.0.0.1";
	cur_request.port = "8080";
	cur_request.target = "/redis/post/";
	cur_request.version = http_utils::common::http_version::v1_1;
	cur_request.method = http::verb::post;
	json body;
	body["request_id"] = "redis_request_one:" + std::to_string(next_cmd_idx);
	std::vector<std::string> temp_vec;
	temp_vec.push_back(cmds[next_cmd_idx]);
	body["redis_cmds"] = temp_vec;
	body["channel"] = "redis_request_one";
	auto result_lambda = [&](http_utils::common::error_pos ec, const http::response<http::string_body>& response) mutable
	{
		if (ec != http_utils::common::error_pos::ok)
		{
			cur_logger->info("request error {}", magic_enum::enum_name(ec));
		}
		cur_logger->info("cmd {} reply {}", cmds[next_cmd_idx - 1], response.body());
		make_request(ioc, cur_logger, host, port, expire_time, next_cmd_idx, cmds);
	};
	auto cur_callback = std::make_shared<http_utils::common::callback_t>(result_lambda);
	auto cur_session = std::make_shared<http_utils::client::session>(ioc, cur_request, cur_logger, cur_callback, expire_time);

	cur_logger->info("make request index {} content {}", next_cmd_idx, cmds[next_cmd_idx]);
	next_cmd_idx++;

	cur_session->run();
}
int main(int argc, const char** argv)
{
	std::string argv_info = "args format: host port expire_time";
	if (argc != 4)
	{
		std::cout << argv_info << std::endl;
		return 0;
	}
	std::string host = std::string(argv[1]);
	std::uint16_t port = std::stoi(argv[2]);
	std::uint32_t expire_time = std::stoi(argv[3]);
	std::vector<std::string> detail_cmds;
	std::vector<std::vector<std::string>> batch_cmds;
	std::vector<std::string> temp_cmds;

	temp_cmds = create_hash_cmds();
	detail_cmds.insert(detail_cmds.end(), temp_cmds.begin(), temp_cmds.end());
	batch_cmds.push_back(temp_cmds);

	temp_cmds = create_list_cmds();
	detail_cmds.insert(detail_cmds.end(), temp_cmds.begin(), temp_cmds.end());
	batch_cmds.push_back(temp_cmds);

	temp_cmds = create_zset_cmds();
	detail_cmds.insert(detail_cmds.end(), temp_cmds.begin(), temp_cmds.end());
	batch_cmds.push_back(temp_cmds);

	temp_cmds = create_string_cmds();
	detail_cmds.insert(detail_cmds.end(), temp_cmds.begin(), temp_cmds.end());
	batch_cmds.push_back(temp_cmds);

	temp_cmds = create_set_cmds();
	detail_cmds.insert(detail_cmds.end(), temp_cmds.begin(), temp_cmds.end());
	batch_cmds.push_back(temp_cmds);

	net::io_context ioc;
	auto cur_logger = create_logger("redis_client");

	std::uint32_t next_cmd_idx = 0;
	make_request(ioc, cur_logger, host, port, expire_time, next_cmd_idx, detail_cmds);
	ioc.run();

}