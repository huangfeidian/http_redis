#include <command/string.h>
using namespace spiritsaway::http_redis::command;

std::string key::set(const std::string& key, const std::string& value, std::uint32_t ttl_seconds, set_mode mode)
{
	std::vector<std::string> result;
	result.push_back("SET");
	result.push_back(key);
	result.push_back(value);
	if(ttl_seconds != 0)
	{
		result.push_back("EX " + std::to_string(ttl_seconds));
	}
	switch (mode)
	{
	case set_mode::nx:
		result.push_back("NX");
		break;
	case set_mode::xx:
		result.push_back("XX");
		break;			
	default:
		break;
	}
	return cmd_join(result);
}
std::string key::set(const std::vector<std::pair<std::string, std::string>>& kvs, bool nx_flag)
{
	std::vector<std::string> result;
	result.reserve(2 * kvs.size() + 2);
	if (nx_flag)
	{
		result.push_back("MSETNX");
	}
	else
	{
		result.push_back("MSET");

	}

	for (const auto&[k, v] : kvs)
	{
		result.push_back(k);
		result.push_back(v);
	}
	return cmd_join(result);
}

std::string key::set(const std::unordered_map<std::string, std::string>& kvs, bool nx_flag)
{
	std::vector<std::string> result;
	result.reserve(2 * kvs.size() + 2);
	if (nx_flag)
	{
		result.push_back("MSETNX");
	}
	else
	{
		result.push_back("MSET");

	}

	for (const auto&[k, v] : kvs)
	{
		result.push_back(k);
		result.push_back(v);
	}
	return cmd_join(result);
}

std::string key::get(const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.reserve(keys.size() + 2);
	result.push_back("MGET");
	result.insert(result.end(), keys.begin(), keys.end());

	return cmd_join(result);
}

std::string key::get(const std::string& key)
{
	return "GET " + key;
}
std::string key::getset(const std::string& key, const std::string& value)
{
	return "GETSET " + key + " " + value;
}

std::string key::expire(const std::string& key, std::uint32_t ttl_seconds)
{
	return "EXPIRE " + key + " " + std::to_string(ttl_seconds);
}
std::string key::expireat(const std::string& key, std::uint32_t utc_timstamp)
{
	return "EXPIREAT " + key + " " + std::to_string(utc_timstamp);

}

std::string key::del(const std::string& key)
{
	return "DEL " + key;
}

std::string key::del(const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.reserve(keys.size() + 2);
	result.push_back("DEL");
	for(const auto& one_key: keys)
	{
		result.push_back(one_key);
	}
	return cmd_join(result);

}

std::string key::persist(const std::string& key)
{
	return "PERSIST " + key;
}

std::string key::ttl(const std::string& key)
{
	return "TTL " + key;
}

std::string key::strlen(const std::string& key)
{
	return "STRLEN " + key;
}
std::string key::append(const std::string& key, const std::string& value)
{
	return "APPEND " + key + " " + value;
}
std::string key::increase(const std::string& key)
{
	return "INCR " + key;

}
std::string key::increase(const std::string& key, int value)
{
	return "INCRBY " + key + " " + std::to_string(value);
}

std::string key::increase(const std::string& key, double value)
{
	return "INCRBYFLOAT " + key + " " + std::to_string(value);
}

std::string key::decrease(const std::string& key)
{
	return "DECR " + key;

}
std::string key::decrease(const std::string& key, int value)
{
	return "DECRBY " + key + " " + std::to_string(value);
}
