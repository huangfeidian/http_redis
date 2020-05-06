#pragma once
#include "cmd_utils.h"

namespace spiritsaway::http_redis::command
{
	class set
	{
		public:
		static std::string add(const std::string& key, const std::string& value);
		static std::string add(const std::string& key, const std::vector<std::string>& values);

		static std::string size(const std::string& key);
		static std::string diff(const std::string& key, const std::string& key2);
		static std::string diff(const std::vector<std::string>& keys);
		static std::string diff_store(const std::string& dest, const std::vector<std::string>& keys);
		static std::string intersect(const std::string& key, const std::string& key2);
		static std::string intersect(const std::vector<std::string>& keys);

		static std::string intersect_store(const std::string& dest, const std::vector<std::string>& keys);

		static std::string unions(const std::string& key, const std::string& key2);
		static std::string unions(const std::vector<std::string>& keys);
		static std::string union_store(const std::string& dest, const std::vector<std::string>& keys);

		static std::string exist(const std::string& key, const std::string& value);
		static std::string move(const std::string& from, const std::string& dest,  const std::string& value);

		static std::string pop_random(const std::string& key, std::uint32_t count = 1);

		static std::string random_keys(const std::string& key, std::uint32_t count = 1);

		static std::string remove(const std::string& key, const std::string& value);

		static std::string remove(const std::string& key, const std::vector<std::string>& values);
		static std::string values(const std::string& key);
		static std::string scan(const std::string& name, std::uint32_t cursor, std::string pattern, std::uint32_t count = 10);

	};
}