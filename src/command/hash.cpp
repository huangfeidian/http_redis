#include <command/hash.h>

using namespace spiritsaway::http_redis::command;

std::string hash::set(const std::string& name, const std::string& key, const std::string& value)
{
	std::vector<std::string> result;
	result.push_back("HSET");
	result.push_back(name);
	result.push_back(key);
	result.push_back(value);
	return cmd_join(result);
}
std::string hash::setnx(const std::string& name, const std::string& key, const std::string& value)
{
	std::vector<std::string> result;
	result.push_back("HSETNX");
	result.push_back(name);
	result.push_back(key);
	result.push_back(value);
	return cmd_join(result);
}
std::string hash::set(const std::string& name, const std::vector<std::pair<std::string, std::string>>& kvs)
{
	std::vector<std::string> result;
	result.reserve(2*kvs.size() + 2);
	result.push_back("HMSET");
	result.push_back(name);
	for(const auto& [k, v]: kvs)
	{
		result.push_back(k);
		result.push_back(v);
	}
	
	return cmd_join(result);
}
std::string hash::get(const std::string& name, const std::string& key)
{
	std::vector<std::string> result;
	result.push_back("HGET");
	result.push_back(name);
	result.push_back(key);
	return cmd_join(result);
}

std::string hash::get(const std::string& name, const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.push_back("HMGET");
	result.push_back(name);
	result.insert(result.end(), keys.begin(), keys.end());
	return cmd_join(result);
}



std::string hash::exists(const std::string& name, const std::string& key)
{
	std::vector<std::string> result;
	result.push_back("HEXISTS");
	result.push_back(name);
	result.push_back(key);
	return cmd_join(result);
}
std::string hash::del(const std::string& name, const std::string& key)
{
	std::vector<std::string> result;
	result.push_back("HDEL");
	result.push_back(name);
	result.push_back(key);
	return cmd_join(result);
}
std::string hash::del(const std::string& name, const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.push_back("HDEL");
	result.push_back(name);
	result.insert(result.end(), keys.begin(), keys.end());
	return cmd_join(result);
}
std::string hash::size(const std::string& name)
{
	return "HLEN " + name;
}
std::string hash::value_strlen(const std::string& name, const std::string& key)
{
	std::vector<std::string> result;
	result.push_back("HSTRLEN");
	result.push_back(name);
	result.push_back(key);
	return cmd_join(result);
}
std::string hash::increase(const std::string& name, const std::string& key, std::int64_t value)
{
	std::vector<std::string> result;
	result.push_back("HINCRBY");
	result.push_back(name);
	result.push_back(key);
	result.push_back(std::to_string(value));
	return cmd_join(result);
}

std::string hash::increase(const std::string& name, const std::string& key, double value)
{
	std::vector<std::string> result;
	result.push_back("HINCRBYFLOAT");
	result.push_back(name);
	result.push_back(key);
	result.push_back(std::to_string(value));
	return cmd_join(result);
}


std::string hash::keys(const std::string& name)
{
	return "HKEYS " + name;
}

std::string hash::values(const std::string& name)
{
	return "HVALS " + name;
}

std::string hash::key_values(const std::string& name)
{
	return "HGETALL " + name;
}
std::string hash::scan(const std::string& name, std::uint32_t cursor, std::string pattern, std::uint32_t count)
{
	std::vector<std::string> result;
	result.reserve(10);
	result.push_back("HSCAN");
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
