#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

namespace spiritsaway::http_redis
{
	class reply
	{
	public:
		std::vector<std::string> content;
		std::string error;
		bool is_array;
	};

	class task
	{
		public:
		using channel_type = std::string;
		using callback_t = std::function<void(const std::vector<reply>&)>;
	protected:
		channel_type _channel;
		std::vector<std::string> _cmds;
		callback_t _callback;
		std::string _request_id;
	public:
		task(const channel_type in_channel,
			const std::vector<std::string>& in_cmds,
			const std::string& in_request_id,
			callback_t in_callback)
			: _channel(in_channel)
			, _cmds(in_cmds)
			, _request_id(in_request_id)
			, _callback(in_callback)
		{

		}
		task(const task& other) = delete;
		task& operator=(const task& other) = delete;
		const std::vector<std::string>& cmds() const
		{
			return _cmds;
		}
		const channel_type& channel_id() const
		{
			return _channel;
		}
		const std::string& request_id() const
		{
			return _request_id;
		}
		void finish(const std::vector<reply>& replys)
		{
			if (_callback)
			{
				_callback(replys);
			}

		}
	};

}