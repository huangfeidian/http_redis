#include <command/zset.h>

using namespace spiritsaway::http_redis::command;

std::string zset::add(const std::string& name, const std::string& key, double value)
{
	return "ZADD " + key + " " + std::to_string(value);
}

std::string zset::add(const std::string& name, const std::vector<std::pair<std::string, double>>& kvs)
{
	std::vector<std::string> result;
	result.reserve(2*kvs.size() + 2);
	result.push_back("ZADD");
	result.push_back(name);
	for(const auto& [k, v] : kvs)
	{
		result.push_back(std::to_string(v));
		result.push_back(k);

	}
	return cmd_join(result);
}

std::string zset::add(const std::string& name, const std::unordered_map<std::string, double>& kvs)
{
	std::vector<std::string> result;
	result.reserve(2*kvs.size() + 2);
	result.push_back("ZADD");
	result.push_back(name);
	for(const auto& [k, v] : kvs)
	{
		result.push_back(std::to_string(v));
		result.push_back(k);

	}
	return cmd_join(result);
}

std::string zset::add(const std::string& name, const std::map<std::string, double>& kvs)
{
	std::vector<std::string> result;
	result.reserve(2*kvs.size() + 2);
	result.push_back("ZADD");
	result.push_back(name);
	for(const auto& [k, v] : kvs)
	{
		result.push_back(std::to_string(v));
		result.push_back(k);

	}
	return cmd_join(result);
}

std::string zset::add(const std::string& name, const std::string& key, std::int64_t value)
{
	return "ZADD " + key + " " + std::to_string(value);
}

std::string zset::add(const std::string& name, const std::vector<std::pair<std::string, std::int64_t>>& kvs)
{
	std::vector<std::string> result;
	result.reserve(2*kvs.size() + 2);
	result.push_back("ZADD");
	result.push_back(name);
	for(const auto& [k, v] : kvs)
	{
		result.push_back(std::to_string(v));
		result.push_back(k);

	}
	return cmd_join(result);
}

std::string zset::add(const std::string& name, const std::unordered_map<std::string, std::int64_t>& kvs)
{
	std::vector<std::string> result;
	result.reserve(2*kvs.size() + 2);
	result.push_back("ZADD");
	result.push_back(name);
	for(const auto& [k, v] : kvs)
	{
		result.push_back(std::to_string(v));
		result.push_back(k);

	}
	return cmd_join(result);
}

std::string zset::add(const std::string& name, const std::map<std::string, std::int64_t>& kvs)
{
	std::vector<std::string> result;
	result.reserve(2*kvs.size() + 2);
	result.push_back("ZADD");
	result.push_back(name);
	for(const auto& [k, v] : kvs)
	{
		result.push_back(std::to_string(v));
		result.push_back(k);

	}
	return cmd_join(result);
}


std::string zset::rank(const std::string& name, const std::string& key)
{
	return "ZRANK " + name + " " + key;
}

std::string zset::reverse_rank(const std::string& name, const std::string& key)
{
	return "ZREVRANK " + name + " " + key;
}

std::string zset::score(const std::string& name, const std::string& key)
{
	return "ZSCORE " + name + " " + key;
}



std::string zset::size(const std::string& name)
{
	return "ZCARD " + name;

}


std::string zset::value_count(const std::string& name, double min, double max)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("ZCOUNT");
	result.push_back(name);
	result.push_back(std::to_string(min));

	result.push_back(std::to_string(max));
	return cmd_join(result);
}

std::string zset::value_count(const std::string& name, std::int64_t min, std::int64_t max)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("ZCOUNT");
	result.push_back(name);
	result.push_back(std::to_string(min));

	result.push_back(std::to_string(max));
	return cmd_join(result);
}

std::string zset::key_count(const std::string& name, const std::string& min, const std::string& max)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("ZLEXCOUNT");
	result.push_back(name);
	result.push_back(min);

	result.push_back(max);
	return cmd_join(result);
}

std::string zset::increase(const std::string& name, const std::string& key, double value)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("ZINCRBY");
	result.push_back(name);
	result.push_back(std::to_string(value));

	result.push_back(key);
	return cmd_join(result);
}

std::string zset::increase(const std::string& name, const std::string& key, std::int64_t value)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("ZINCRBY");
	result.push_back(name);
	result.push_back(std::to_string(value));

	result.push_back(key);
	return cmd_join(result);
}

std::string zset::intersection_store(const std::string& dest, const std::string& key_1, const std::string& key_2, aggregate_type cur_type)
{
	std::vector<std::string> result;
	result.reserve(8);
	result.push_back("ZINTERSTORE");
	result.push_back(dest);
	result.push_back(std::to_string(2));
	result.push_back(key_1);
	result.push_back(key_2);
	switch (cur_type)
	{
	case aggregate_type::min:
		result.push_back("AGGREGATE MIN");
		break;
	case aggregate_type::max:
		result.push_back("AGGREGATE MAX");
		break;
	default:
		break;
	}
	return cmd_join(result);
}

std::string zset::intersection_store(const std::string& dest, const std::vector<std::string>& keys, aggregate_type cur_type)
{
	std::vector<std::string> result;
	result.reserve(6 + keys.size());
	result.push_back("ZINTERSTORE");
	result.push_back(dest);
	result.push_back(std::to_string(keys.size()));
	result.insert(result.end(), keys.begin(), keys.end());
	switch (cur_type)
	{
	case aggregate_type::min:
		result.push_back("AGGREGATE MIN");
		break;
	case aggregate_type::max:
		result.push_back("AGGREGATE MAX");
		break;
	default:
		break;
	}
	return cmd_join(result);
}

std::string zset::intersection_store(const std::string& dest, const std::vector<std::string>& keys, const std::vector<double>& weights, aggregate_type cur_type)
{
	std::vector<std::string> result;
	result.reserve(10 + keys.size() * 2);
	result.push_back("ZINTERSTORE");
	result.push_back(dest);
	result.push_back(std::to_string(keys.size()));
	result.insert(result.end(), keys.begin(), keys.end());
	result.push_back("WEIGHTS");
	for(auto one_weight: weights)
	{
		result.push_back(std::to_string(one_weight));
	}
	switch (cur_type)
	{
	case aggregate_type::min:
		result.push_back("AGGREGATE MIN");
		break;
	case aggregate_type::max:
		result.push_back("AGGREGATE MAX");
		break;
	default:
		break;
	}
	return cmd_join(result);
}
std::string zset::union_store(const std::string& dest, const std::string& key_1, const std::string& key_2, aggregate_type cur_type)
{
	std::vector<std::string> result;
	result.reserve(8);
	result.push_back("ZUNIONSTORE");
	result.push_back(dest);
	result.push_back(std::to_string(2));
	result.push_back(key_1);
	result.push_back(key_2);
	switch (cur_type)
	{
	case aggregate_type::min:
		result.push_back("AGGREGATE MIN");
		break;
	case aggregate_type::max:
		result.push_back("AGGREGATE MAX");
		break;
	default:
		break;
	}
	return cmd_join(result);
}

std::string zset::union_store(const std::string& dest, const std::vector<std::string>& keys, aggregate_type cur_type)
{
	std::vector<std::string> result;
	result.reserve(6 + keys.size());
	result.push_back("ZUNIONSTORE");
	result.push_back(dest);
	result.push_back(std::to_string(keys.size()));
	result.insert(result.end(), keys.begin(), keys.end());
	switch (cur_type)
	{
	case aggregate_type::min:
		result.push_back("AGGREGATE MIN");
		break;
	case aggregate_type::max:
		result.push_back("AGGREGATE MAX");
		break;
	default:
		break;
	}
	return cmd_join(result);
}

std::string zset::union_store(const std::string& dest, const std::vector<std::string>& keys, const std::vector<double>& weights, aggregate_type cur_type)
{
	std::vector<std::string> result;
	result.reserve(10 + keys.size() * 2);
	result.push_back("ZUNIONSTORE");
	result.push_back(dest);
	result.push_back(std::to_string(keys.size()));
	result.insert(result.end(), keys.begin(), keys.end());
	result.push_back("WEIGHTS");
	for(auto one_weight: weights)
	{
		result.push_back(std::to_string(one_weight));
	}
	switch (cur_type)
	{
	case aggregate_type::min:
		result.push_back("AGGREGATE MIN");
		break;
	case aggregate_type::max:
		result.push_back("AGGREGATE MAX");
		break;
	default:
		break;
	}
	return cmd_join(result);
}

std::string zset::rank_range(const std::string& name, int start, int stop, bool withscores)
{
	std::vector<std::string> result;
	result.reserve(5);
	result.push_back("ZRANGE");
	result.push_back(name);
	result.push_back(std::to_string(start));
	result.push_back(std::to_string(stop));

	if(withscores)
	{
		result.push_back("WITHSCORES");
	}
	return cmd_join(result);
}

std::string zset::reverse_rank_range(const std::string& name, std::uint32_t start, std::uint32_t stop, bool withscores)
{
	std::vector<std::string> result;
	result.reserve(5);
	result.push_back("ZREVRANGE");
	result.push_back(name);
	result.push_back(std::to_string(start));
	result.push_back(std::to_string(stop));

	if(withscores)
	{
		result.push_back("WITHSCORES");
	}
	return cmd_join(result);
}

std::string zset::score_range(const std::string& name, double min, double max, bool withscores, std::uint32_t offset, std::uint32_t count)
{
	std::vector<std::string> result;
	result.reserve(10);
	result.push_back("ZRANGEBYSCORE");
	result.push_back(name);
	result.push_back(std::to_string(min));
	result.push_back(std::to_string(max));

	if(withscores)
	{
		result.push_back("WITHSCORES");
	}
	if(count != 0)
	{
		result.push_back("LIMIT");
		result.push_back(std::to_string(offset));
		result.push_back(std::to_string(count));
	}
	return cmd_join(result);
}

std::string zset::score_range(const std::string& name, std::int64_t min, std::int64_t max, bool withscores, std::uint32_t offset, std::uint32_t count)
{
	std::vector<std::string> result;
	result.reserve(10);
	result.push_back("ZRANGEBYSCORE");
	result.push_back(name);
	result.push_back(std::to_string(min));
	result.push_back(std::to_string(max));

	if(withscores)
	{
		result.push_back("WITHSCORES");
	}
	if(count != 0)
	{
		result.push_back("LIMIT");
		result.push_back(std::to_string(offset));
		result.push_back(std::to_string(count));
	}
	return cmd_join(result);
}

std::string zset::key_range(const std::string& name, const std::string& min, const std::string& max, std::uint32_t offset, std::uint32_t count)
{
	std::vector<std::string> result;
	result.reserve(8);
	result.push_back("ZRANGEBYLEX");
	result.push_back(name);
	result.push_back(min);
	result.push_back(max);

	if(count != 0)
	{
		result.push_back("LIMIT");
		result.push_back(std::to_string(offset));
		result.push_back(std::to_string(count));
	}
	return cmd_join(result);
}


std::string zset::remove(const std::string& key, const std::string& value)
{
	return "ZREM" + key + " " + value;
}

std::string zset::remove(const std::string& key, const std::vector<std::string>& values)
{
	std::vector<std::string> result;
	result.reserve(values.size() + 2);
	result.push_back("ZREM");
	result.insert(result.end(), values.begin(), values.end());
	return cmd_join(result);
}

std::string zset::remove_key_range(const std::string& name, const std::string& min, const std::string& max)
{
	std::vector<std::string> result;
	result.reserve(8);
	result.push_back("ZREMRANGEBYLEX");
	result.push_back(name);
	result.push_back(min);
	result.push_back(max);
	return cmd_join(result);
}
std::string zset::remove_rank_range(const std::string& name, int start, int stop)
{
	std::vector<std::string> result;
	result.reserve(5);
	result.push_back("ZREMRANGEBYRANK");
	result.push_back(name);
	result.push_back(std::to_string(start));
	result.push_back(std::to_string(stop));

	return cmd_join(result);
}

std::string zset::remove_score_range(const std::string& name, double min, double max)
{
	std::vector<std::string> result;
	result.reserve(10);
	result.push_back("ZREMRANGEBYSCORE");
	result.push_back(name);
	result.push_back(std::to_string(min));
	result.push_back(std::to_string(max));

	return cmd_join(result);
}

std::string zset::remove_score_range(const std::string& name, std::int64_t min, std::int64_t max)
{
	std::vector<std::string> result;
	result.reserve(10);
	result.push_back("ZREMRANGEBYSCORE");
	result.push_back(name);
	result.push_back(std::to_string(min));
	result.push_back(std::to_string(max));

	return cmd_join(result);
}
std::string zset::scan(const std::string& name, std::uint32_t cursor, std::string pattern, std::uint32_t count)
{
	std::vector<std::string> result;
	result.reserve(10);
	result.push_back("ZSCAN");
	result.push_back(name);
	result.push_back(std::to_string(cursor));
	if(!pattern.empty())
	{
		result.push_back("MATCH");
		result.push_back(pattern);
	}
	result.push_back("COUNT");
	result.push_back(std::to_string(count));

	return cmd_join(result);
}
