#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace spiritsaway::http_redis
{
	class reply
	{
	public:
		std::vector<std::string> content;
		std::string error;
		bool is_array;
	};
	struct task_desc
	{
		std::string channel;
		std::vector<std::string> cmds;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(task_desc, channel, cmds)
	};
	class task
	{
		public:
		using channel_type = std::string;
		using callback_t = std::function<void(const std::vector<reply>&)>;
	protected:
		const task_desc m_desc;
		callback_t m_callback;
	public:
		task(const task_desc cur_desc,
			callback_t in_callback)
			: m_desc(cur_desc)
			, m_callback(in_callback)
		{

		}
		task(const task& other) = delete;
		task& operator=(const task& other) = delete;
		const task_desc& desc() const
		{
			return m_desc;
		}
		void finish(const std::vector<reply>& replys)
		{
			if (m_callback)
			{
				m_callback(replys);
			}

		}
		const std::string& channel_id() const
		{
			return m_desc.channel;
		}
	};

}