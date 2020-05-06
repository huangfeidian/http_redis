
#pragma once
#include "cmd_utils.h"

namespace spiritsaway::http_redis::command
{
	class hash
	{
	public:
		static std::string set(const std::string& name, const std::string& key, const std::string& value);
		static std::string set(const std::string& name, const std::vector<std::pair<std::string, std::string>>& kvs);
		static std::string get(const std::string& name, const std::string& key);
		static std::string get(const std::string& name, const std::vector<std::string>& keys);

		static std::string setnx(const std::string& name, const std::string& key, const std::string& value);
		static std::string exists(const std::string& name, const std::string& key);
		static std::string del(const std::string& name, const std::string& key);
		static std::string del(const std::string& name, const std::vector<std::string>& keys);
		static std::string size(const std::string& name);
		static std::string value_strlen(const std::string& name, const std::string& key);
		static std::string increase(const std::string& name, const std::string& key, std::int64_t value);

		static std::string increase(const std::string& name, const std::string& key, double value);

		static std::string keys(const std::string& name);

		static std::string values(const std::string& name);
		static std::string key_values(const std::string& name);
		static std::string scan(const std::string& name, std::uint32_t cursor, std::string pattern, std::uint32_t count = 10);

	};
}