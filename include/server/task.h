#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

namespace spiritsaway::http_redis
{
	class task
	{
		public:
		using channel_type = std::string;
		using callback_t = std::function<void(const std::string& error, const std::vector<std::string>&, bool)>;
	protected:
		channel_type _channel;
		std::vector<std::string> _cmds;
		std::weak_ptr<callback_t> _callback;
	public:
		task(const channel_type in_channel,
			const std::vector<std::string>& in_cmds,
			std::weak_ptr<callback_t> in_callback)
			: _channel(in_channel)
			, _cmds(in_cmds)
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
		void finish(const std::string& error, const std::vector<std::string>& content, bool is_array)
		{
			auto cur_callback = _callback.lock();
			_callback.reset();
			cur_callback->operator()(error, content, is_array);

		}
	};
}