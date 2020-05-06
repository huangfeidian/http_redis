#pragma once

#include "cmd_utils.h"

namespace spiritsaway::http_redis::command
{
	class list
	{
	public:
		static std::string push_front(const std::string& key, const std::string& value);

		static std::string push_front(const std::string& key, const std::vector<std::string>& values);
		static std::string push_front_ex(const std::string& key, const std::string& value);

		static std::string push_back(const std::string& key, const std::string& value);

		static std::string push_back(const std::string& key, const std::vector<std::string>& values);
		static std::string push_back_ex(const std::string& key, const std::string& value);
		static std::string pop_front(const std::string& key);
		static std::string pop_back(const std::string& key);

		static std::string pop_back_and_push_front(const std::string& key1, const std::string& key2);

		static std::string remove_value_all(const std::string& key, const std::string value);

		static std::string remove_value(const std::string& key, std::uint32_t count, const std::string value);

		static std::string remove_value_reverse(const std::string& key, std::uint32_t count, const std::string value);

		static std::string size(const std::string key);

		static std::string value_at(const std::string& key, int index);

		static std::string insert_before(const std::string& key, const std::string& value);

		static std::string insert_after(const std::string& key, const std::string& value);

		static std::string value_set(const std::string& key, int index, const std::string& value);

		static std::string itervalues(const std::string& key, int start, int stop);
		static std::string values(const std::string& key);
		static std::string trim(const std::string& key, int start, int stop);

	};
}