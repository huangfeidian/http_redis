#pragma once

#include "worker/task.h"

#include "worker/worker.h"

#include <http_utils/http_server.h>

#include <nlohmann/json.hpp>

namespace spiritsaway::http_redis
{


	class redis_server : public http_utils::http_server
	{
	public:
		redis_server(asio::io_context& ioc, std::shared_ptr<spdlog::logger> in_logger, const std::string& address, const std::string& port, concurrency::task_channels<task, true>& task_dest);
	protected:
		void handle_request(const http_utils::request& req, http_utils::reply_handler rep_cb) override;
		std::string check_request(const http_utils::request& cur_req, task_desc& cur_task_desc) const;
		void finish_task(const http_utils::request& cur_req, const std::vector<reply>& replys, http_utils::reply_handler rep_cb);
	private:
		spiritsaway::concurrency::task_channels<http_redis::task, true>& m_task_dest;
	};


}