#include <server/http_server.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/ostr.h>

using namespace spiritsaway;
using namespace http_utils;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

std::shared_ptr<spdlog::logger> create_logger(const std::string& name)
{
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::debug);
	std::string pattern = "[thread %t] %+";
	console_sink->set_pattern(pattern);

	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(name + ".log", true);
	file_sink->set_level(spdlog::level::trace);
	file_sink->set_pattern(pattern);
	auto logger = std::make_shared<spdlog::logger>(name, spdlog::sinks_init_list{ console_sink, file_sink });
	logger->set_level(spdlog::level::trace);
	return logger;
}

int main(int argc, const char** argv)
{
	std::string arg_info = "args format: listen_host listen_port redis_host redis_port [io_worker_num = 2] [task_worker_num = 2] [expire_time = 10]  [passwd] ";
	//if (argc < 5)
	//{
	//	std::cout << arg_info << std::endl;
	//	return 0;
	//}
	//std::string listen_host = argv[1];
	//std::uint16_t listen_port = std::stoi(argv[2]);
	//std::string redis_host = argv[3];
	//std::uint16_t redis_port = std::stoi(argv[4]);

	std::string listen_host = "0.0.0.0";
	std::uint16_t listen_port = 8080;
	std::string redis_host = "192.168.1.160";
	std::uint16_t redis_port = 6379;

	std::string passwd = "";
	std::uint32_t expire_time = 10;
	std::uint32_t io_worker_num = 2;
	std::uint32_t task_worker_num = 2;
	//if (argc >= 6)
	//{
	//	io_worker_num = std::stoi(argv[5]);
	//}
	//if (argc >= 7)
	//{
	//	task_worker_num = std::stoi(argv[6]);
	//}
	//if (argc >= 8)
	//{
	//	expire_time = std::max(1, std::stoi(argv[7]));
	//}
	//if (argc >= 9)
	//{
	//	passwd = argv[8];
	//}
	http_redis::redis_config cur_redis_config;
	cur_redis_config.host = redis_host;
	cur_redis_config.port = redis_port;
	cur_redis_config.passwd = passwd;
	cur_redis_config.timeout = std::max(1u, expire_time / 2);
	auto listen_address = net::ip::make_address(listen_host);
	net::io_context ioc{ static_cast<int>(io_worker_num) };
	concurrency::task_channels<http_redis::task, true> task_queue;
	auto cur_logger = create_logger("redis_http_server");
	auto cur_listener = std::make_shared<http_redis::redis_listener>(ioc, 
		tcp::endpoint(listen_address, listen_port), cur_logger, expire_time, task_queue);
	cur_listener->run();
	std::vector<std::thread> io_worker_threads;
	std::vector<std::thread> task_worker_threads;
	std::vector<std::shared_ptr<http_redis::worker>> task_workers;
	for (int i = 0; i < task_worker_num; i++)
	{
		auto cur_task_worker = std::make_shared<http_redis::worker>(cur_redis_config, task_queue, cur_logger);
		task_workers.push_back(cur_task_worker);
		task_worker_threads.emplace_back([cur_task_worker]()
		{
			cur_task_worker->run();
		});
	}
	for (int i = 0; i + 1 < io_worker_num; i++)
	{
		io_worker_threads.emplace_back([&ioc]()
		{
			ioc.run();
		});
	}
	ioc.run();


}