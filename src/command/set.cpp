#include <command/set.h>

using namespace spiritsaway::http_redis::command;

std::string set::add(const std::string& key, const std::string& value)
{
	return "SADD " + key + " " + value;
}
std::string set::add(const std::string& key, const std::vector<std::string>& values)
{
	std::vector<std::string> result;
	result.reserve(values.size() + 2);
	result.push_back("SADD");
	result.push_back(key);
	result.insert(result.end(), values.begin(), values.end());

	return cmd_join(result);
}

std::string set::size(const std::string& key)
{
	return "SCARD " + key;
}
std::string set::diff(const std::string& key, const std::string& key2)
{
	return "SDIFF " + key + " " + key2;
}
std::string set::diff(const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.reserve(keys.size() + 2);
	result.push_back("SDIFF");
	result.insert(result.end(), keys.begin(), keys.end());
	return cmd_join(result);
}
std::string set::diff_store(const std::string& dest, const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.reserve(keys.size() + 2);
	result.push_back("SDIFFSTORE");
	result.push_back(dest);
	result.insert(result.end(), keys.begin(), keys.end());
	return cmd_join(result);
}
std::string set::intersect(const std::string& key, const std::string& key2)
{
	return "SINTER  " + key + " " + key2;
}
std::string set::intersect(const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.reserve(keys.size() + 2);
	result.push_back("SINTER ");
	result.insert(result.end(), keys.begin(), keys.end());
	return cmd_join(result);
}

std::string set::intersect_store(const std::string& dest, const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.reserve(keys.size() + 2);
	result.push_back("SINTERSTORE");
	result.push_back(dest);
	result.insert(result.end(), keys.begin(), keys.end());
	return cmd_join(result);
}

std::string set::unions(const std::string& key, const std::string& key2)
{
	return "SUNION " + key + " " + key2;
}
std::string set::unions(const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.reserve(keys.size() + 2);
	result.push_back("SUNION");
	result.insert(result.end(), keys.begin(), keys.end());
	return cmd_join(result);
}
std::string set::union_store(const std::string& dest, const std::vector<std::string>& keys)
{
	std::vector<std::string> result;
	result.reserve(keys.size() + 2);
	result.push_back("SUNIONSTORE");
	result.push_back(dest);
	result.insert(result.end(), keys.begin(), keys.end());
	return cmd_join(result);
}

std::string set::exist(const std::string& key, const std::string& value)
{
	return "SISMEMBER " + key + " " + value;
}
std::string set::move(const std::string& from, const std::string& dest,  const std::string& value)
{
	std::vector<std::string> result;
	result.push_back("SMOVE");
	result.push_back(from);
	result.push_back(dest);
	result.push_back(value);
	return cmd_join(result);
}

std::string set::pop_random(const std::string& key, std::uint32_t count)
{
	return "SPOP " + key + " " + std::to_string(count);
}

std::string set::random_keys(const std::string& key, std::uint32_t count )
{
	return "SRANDMEMBER " + key + " " + std::to_string(count);
}

std::string set::remove(const std::string& key, const std::string& value)
{
	return "SREM " + key + " " + value;
}

std::string set::remove(const std::string& key, const std::vector<std::string>& values)
{
	std::vector<std::string> result;
	result.reserve(values.size() + 2);
	result.push_back("SREM  ");
	result.insert(result.end(), values.begin(), values.end());
	return cmd_join(result);
}
std::string set::values(const std::string& key)
{
	return "SMEMBERS " + key;

}
std::string set::scan(const std::string& name, std::uint32_t cursor, std::string pattern, std::uint32_t count)
{
	std::vector<std::string> result;
	result.reserve(10);
	result.push_back("SSCAN");
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