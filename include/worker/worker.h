
#pragma once

#include <hiredis/hiredis.h>
#include <task_channel/task_channel.h>
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <nlohmann/json.hpp>

#include "task.h"


namespace spiritsaway::http_redis
{
	using namespace spiritsaway;
	using logger_t = std::shared_ptr<spdlog::logger>;
	struct redis_config
	{
		std::string host;
		std::uint16_t port;
		std::uint32_t timeout;
		std::string passwd;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(redis_config, host, port,  passwd, timeout)
	};
	class worker
	{
		const redis_config m_config;
		redisContext* ctx = nullptr;
		concurrency::task_channels<task, true>& m_task_source;
		logger_t logger;
		task::channel_type pre_channel;
		std::uint32_t worker_id = 0;
		bool m_stopped = false;
	public:
		worker(const redis_config& config, concurrency::task_channels<task, true>& task_source, logger_t in_logger);
		void run();
		void set_executor_id(std::uint32_t executor_id);
		void notify_stop();
	protected:


		bool connect();
		bool auth();
		void poll();
		bool ping();
		void init_ctx();
		reply handle_reply(const redisReply& cur_reply);
	public:
		virtual ~worker();
	};
}