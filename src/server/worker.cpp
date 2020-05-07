#include <server/worker.h>
#ifdef _MSC_VER
#include <winsock.h>
#endif
using namespace spiritsaway;

using namespace spiritsaway::http_redis;
worker::worker(const redis_config& config, 
	concurrency::task_channels<task>& task_source, 
	logger_t in_logger)
	: _config(config)
	, _task_source(task_source)
	, logger(in_logger)
{

}
void worker::run()
{
	while (true)
	{
		init_ctx();
		poll();
	}
}
void worker::set_executor_id(std::uint32_t executor_id)
{
	worker_id = executor_id;
}
bool worker::connect()
{
	if (ctx != nullptr)
	{
		redisFree(ctx);
		ctx = nullptr;
	}
	struct timeval timeoutVal;
	timeoutVal.tv_sec = _config.timeout;
	timeoutVal.tv_usec = 0;
	logger->info("begin connect to {}:{} with timeout {}", _config.host, _config.port, _config.timeout);
	ctx = redisConnectWithTimeout(_config.host.c_str(), _config.port, timeoutVal);
	if (ctx == nullptr)
	{
		logger->error("fail to connect to {}:{}", _config.host, _config.port);
		return false;
	}
	if (ctx->err)
	{
		logger->error("fail to connect to {}:{} error is {}", _config.host, _config.port, ctx->err);
		redisFree(ctx);
		ctx = nullptr;
		return false;
	}
	logger->info("connect suc");
	return true;
}

bool worker::auth()
{
	if (_config.passwd.empty())
	{
		logger->info("no passwd provided to auth");
		return true;
	}
	redisReply *reply = static_cast<redisReply *>(redisCommand(ctx, "AUTH %s", _config.passwd.c_str()));
	if (!reply)
	{
		logger->error("auth fail: empty reply");
		freeReplyObject(reply);
		return false;
	}
	if (std::strncmp(reply->str, "OK", 2) != 0)
	{
		logger->error("auth fail with reply: {}", std::string(reply->str, reply->len));
		freeReplyObject(reply);
		return false;
	}
	freeReplyObject(reply);
	logger->info("auth suc");
	return true;
}
void worker::poll()
{
	while (true)
	{
		auto cur_task = _task_source.poll_one_task(pre_channel, worker_id);
		if (!cur_task)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (!ping())
			{
				return;
			}
			continue;
		}
		logger->debug("get task {}", cur_task->request_id());
		pre_channel = cur_task->channel_id();
		for (auto one_cmd : cur_task->cmds())
		{
			redisAppendCommand(ctx, one_cmd.c_str());
		}
		redisReply *raw_reply;
		std::vector<reply> result;
		for(int i = 0; i < cur_task->cmds().size(); i++)
		{
			raw_reply = nullptr;
			redisGetReply(ctx, reinterpret_cast<void**>(&raw_reply));
			if (!raw_reply)
			{
				break;
			}
			else
			{
				result.push_back(handle_reply(*raw_reply));
				freeReplyObject(raw_reply);
				raw_reply = nullptr;
			}
		}
		cur_task->finish(result);
		_task_source.finish_task(cur_task);

	}
}

bool worker::ping()
{
	redisReply *reply = static_cast<redisReply *>(redisCommand(ctx, "PING"));
	std::string error = "";
	bool ping_ret = false;
	if (!reply)
	{
		error = "no reply";
		ping_ret = false;
	}
	else
	{
		if (!reply->str)
		{
			error = "reply is not str";
			ping_ret = false;
		}
		else
		{
			auto cur_reply_str = std::string(reply->str, reply->len);
			if (std::strncmp(cur_reply_str.c_str(), "PONG", 4) != 0)
			{
				error = "reply is " + cur_reply_str + " not PONG ";
				ping_ret = false;
			}
			else
			{
				ping_ret = true;
			}
			freeReplyObject(reply);

		}
	}

	if (!ping_ret)
	{
		logger->info("ping {}:{} with fail error {}", _config.host, _config.port, error);
	}
	return ping_ret;
}

void worker::init_ctx()
{
	std::uint32_t reconnect_gap = 0;
	while (true)
	{
		reconnect_gap += 100;
		if (reconnect_gap > 5000)
		{
			reconnect_gap = 5000;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(reconnect_gap));
		if (!connect())
		{
			continue;
		}
		if (!auth())
		{
			continue;
		}
		break;

	}
}

reply worker::handle_reply(const redisReply& cur_reply)
{
	reply result_reply;
	result_reply.is_array = false;

	switch (cur_reply.type)
	{
	case REDIS_REPLY_ERROR:
	{
		if (cur_reply.str == nullptr)
		{
			result_reply.error = "invalid reply str";
		}
		else
		{
			result_reply.error = std::string(cur_reply.str, cur_reply.len);
		}
		break;
	}
	case REDIS_REPLY_NIL:
	{
		result_reply.content.push_back("");
		break;
	}
	case REDIS_REPLY_STRING:
	case REDIS_REPLY_STATUS:
	{
		std::string reply_str;
		if (cur_reply.str == nullptr)
		{
			reply_str = "invalid reply str";
		}
		else
		{
			reply_str = std::string(cur_reply.str, cur_reply.len);
		}
		result_reply.content.push_back(reply_str);
		break;
	}
	case REDIS_REPLY_INTEGER:
	{
		result_reply.content.push_back(std::to_string(cur_reply.integer));
		break;
	}
	case REDIS_REPLY_ARRAY:
	{
		std::vector<std::string> contents;
		result_reply.is_array = true;
		if (cur_reply.element != nullptr)
		{
			for (std::size_t idx = 0; idx != cur_reply.elements; idx++)
			{
				auto cur_sub_reply_ptr = cur_reply.element[idx];
				contents.push_back(std::string(cur_sub_reply_ptr->str, cur_sub_reply_ptr->len));

			}
		}
		break;
	}
	default:
		result_reply.error = "unknown reply type";
	}
	return result_reply;
}

worker::~worker()
{
	if (ctx)
	{
		redisFree(ctx);
		ctx = nullptr;
	}
}
