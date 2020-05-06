#pragma once

#include "cmd_utils.h"
#include <unordered_map>
#include <map>
namespace spiritsaway::http_redis::command
{
	enum class aggregate_type
	{
		sum,
		min,
		max,
	};
	class zset
	{
	public:
		static std::string add(const std::string& name, const std::string& key, double value);

		static std::string add(const std::string& name, const std::vector<std::pair<std::string, double>>& kvs);

		static std::string add(const std::string& name, const std::unordered_map<std::string, double>& kvs);

		static std::string add(const std::string& name, const std::map<std::string, double>& kvs);

		static std::string add(const std::string& name, const std::string& key, std::int64_t value);

		static std::string add(const std::string& name, const std::vector<std::pair<std::string, std::int64_t>>& kvs);

		static std::string add(const std::string& name, const std::unordered_map<std::string, std::int64_t>& kvs);

		static std::string add(const std::string& name, const std::map<std::string, std::int64_t>& kvs);

		static std::string rank(const std::string& name, const std::string& key);

		static std::string reverse_rank(const std::string& name, const std::string& key);

		static std::string score(const std::string& name, const std::string& key);

		

		static std::string size(const std::string& name);


		static std::string value_count(const std::string& name, double min, double max);
		static std::string value_count(const std::string& name, std::int64_t min, std::int64_t max);


		static std::string key_count(const std::string& name, const std::string& min, const std::string& max);

		static std::string increase(const std::string& name, const std::string& key, double value);
		static std::string increase(const std::string& name, const std::string& key, std::int64_t value);


		static std::string intersection_store(const std::string& dest, const std::string& key_1, const std::string& key_2, aggregate_type cur_type = aggregate_type::sum);

		static std::string intersection_store(const std::string& dest, const std::vector<std::string>& keys, aggregate_type cur_type = aggregate_type::sum);

		static std::string intersection_store(const std::string& dest, const std::vector<std::string>& keys, const std::vector<double>& weights, aggregate_type cur_type = aggregate_type::sum);
		static std::string union_store(const std::string& dest, const std::string& key_1, const std::string& key_2, aggregate_type cur_type = aggregate_type::sum);

		static std::string union_store(const std::string& dest, const std::vector<std::string>& keys, aggregate_type cur_type = aggregate_type::sum);

		static std::string union_store(const std::string& dest, const std::vector<std::string>& keys, const std::vector<double>& weights, aggregate_type cur_type = aggregate_type::sum);

		static std::string rank_range(const std::string& name, int start, int stop, bool withscores = false);

		static std::string reverse_rank_range(const std::string& name, std::uint32_t start, std::uint32_t stop, bool withscores = false);

		static std::string score_range(const std::string& name, double min, double max, bool withscores = false, std::uint32_t offset = 0, std::uint32_t count = 0);
		static std::string score_range(const std::string& name, std::int64_t min, std::int64_t max, bool withscores = false, std::uint32_t offset = 0, std::uint32_t count = 0);
		static std::string key_range(const std::string& name, const std::string& min, const std::string& max, std::uint32_t offset = 0, std::uint32_t count = 0);


		static std::string remove(const std::string& key, const std::string& value);

		static std::string remove(const std::string& key, const std::vector<std::string>& values);

		static std::string remove_key_range(const std::string& name, const std::string& min, const std::string& max);
		static std::string remove_rank_range(const std::string& name, int start, int stop);

		static std::string remove_score_range(const std::string& name, double min, double max);
		static std::string remove_score_range(const std::string& name, std::int64_t min, std::int64_t max);

		static std::string scan(const std::string& name, std::uint32_t cursor, std::string pattern, std::uint32_t count = 10);


	};
}