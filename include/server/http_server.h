#pragma once

#include "worker/task.h"

#include "worker/worker.h"

#include <http_utils/http_server.h>

#include <nlohmann/json.hpp>

namespace spiritsaway::http_redis
{
	using namespace spiritsaway;
	using json = nlohmann::json;
	namespace beast = boost::beast;         // from <boost/beast.hpp>
	namespace http = beast::http;           // from <boost/beast/http.hpp>
	namespace net = boost::asio;            // from <boost/asio.hpp>
	using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
	using logger_t = std::shared_ptr<spdlog::logger>;


	class redis_session: public http_utils::server::session
	{
		concurrency::task_channels<task, true>& m_task_dest;
		task_desc m_task_desc;
		std::string check_request() override;
		void route_request() override;
		void finish_task(const std::vector<reply>& replys);
		std::shared_ptr<task::callback_t> callback;
		std::shared_ptr<boost::asio::steady_timer> expire_timer;
		void on_timeout(const boost::system::error_code& e);

	public:
		redis_session(tcp::socket&& socket,
			logger_t in_logger,
			std::uint32_t in_expire_time,
			concurrency::task_channels<task, true>& in_task_dest);

	};

	class redis_listener : public http_utils::server::listener
	{
	protected:
		concurrency::task_channels<task, true>& m_task_dest;

	public:
		redis_listener(net::io_context& ioc,
			tcp::endpoint endpoint,
			logger_t in_logger,
			std::uint32_t expire_time,
			concurrency::task_channels<task, true>& task_dest);

		std::shared_ptr<http_utils::server::session> make_session(tcp::socket&& socket) override;
	};
}