#pragma once

#include "cmd_utils.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace spiritsaway::http_redis::command
{

	enum class set_mode
	{
		default,
		nx,
		xx,
	};

	class key
	{
	public:
		static std::string set(const std::string& key, const std::string& value, std::uint32_t ttl_miliseconds = 0, set_mode mode = set_mode::default);
		static std::string set(const std::vector<std::pair<std::string, std::string>>& kvs, bool nx_flag = false);
		static std::string set(const std::unordered_map<std::string, std::string>& kvs, bool nx_flag = false);
		
		static std::string get(const std::string& key);
		static std::string get(const std::vector<std::string>& keys);

		static std::string getset(const std::string& key, const std::string& value);

		static std::string expire(const std::string& key, std::uint32_t ttl_seconds);
		static std::string expireat(const std::string& key, std::uint32_t utc_timstamp);

		static std::string del(const std::string& key);
		static std::string del(const std::vector<std::string>& keys);

		static std::string persist(const std::string& key);
		static std::string ttl(const std::string& key);

		static std::string strlen(const std::string& key);
		static std::string append(const std::string& key, const std::string& value);
		static std::string increase(const std::string& key);
		static std::string increase(const std::string& key, int value);

		static std::string increase(const std::string& key, double value);
		static std::string decrease(const std::string& key);
		static std::string decrease(const std::string& key, int value);


	};
}