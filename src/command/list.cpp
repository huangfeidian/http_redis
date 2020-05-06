#include <command/list.h>

using namespace spiritsaway::http_redis::command;

std::string list::push_front(const std::string& key, const std::string& value)
{
	return "LPUSH " + key + " " + value;
}

std::string list::push_front(const std::string& key, const std::vector<std::string>& values)
{
	std::vector<std::string> result;
	result.reserve(values.size() + 2);
	result.push_back("LPUSH");
	result.push_back(key);
	result.insert(result.end(), values.begin(), values.end());
	return cmd_join(result);
}
std::string list::push_front_ex(const std::string& key, const std::string& value)
{
	return "LPUSHX " + key + " " + value;
}

std::string list::push_back(const std::string& key, const std::string& value)
{
	return "RPUSH " + key + " " + value;
}

std::string list::push_back(const std::string& key, const std::vector<std::string>& values)
{
	std::vector<std::string> result;
	result.reserve(values.size() + 2);
	result.push_back("RPUSH");
	result.push_back(key);
	result.insert(result.end(), values.begin(), values.end());
	return cmd_join(result);
}
std::string list::push_back_ex(const std::string& key, const std::string& value)
{
	return "RPUSHX " + key + " " + value;
}
std::string list::pop_front(const std::string& key)
{
	return "LPOP " + key;
}
std::string list::pop_back(const std::string& key)
{
	return "RPOP " + key;
}

std::string list::pop_back_and_push_front(const std::string& key1, const std::string& key2)
{
	return "RPOPLPUSH " + key1 + " " + key2;
}

std::string list::remove_value_all(const std::string& key, const std::string value)
{
	return "LREM  " + key + " 0 " + value;
}

std::string list::remove_value(const std::string& key, std::uint32_t count, const std::string value)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("LREM");
	result.push_back(key);
	result.push_back(std::to_string(count));
	result.push_back(value);
	return cmd_join(result);
}

std::string list::remove_value_reverse(const std::string& key, std::uint32_t count, const std::string value)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("LREM");
	result.push_back(key);
	result.push_back(std::to_string(-1 * count));
	result.push_back(value);
	return cmd_join(result);
}

std::string list::size(const std::string key)
{
	return "LLEN " + key;
}

std::string list::value_at(const std::string& key, int index)
{
	return "LINDEX " + key + " " + std::to_string(index);
}

std::string list::insert_before(const std::string& key, const std::string& value)
{
	return "LINSERT " + key + " BEFORE " + value;

}

std::string list::insert_after(const std::string& key, const std::string& value)
{
	return "LINSERT " + key + " AFTER " + value;
}

std::string list::value_set(const std::string& key, int index, const std::string& value)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("LSET");
	result.push_back(key);
	result.push_back(std::to_string(index));
	result.push_back(value);
	return cmd_join(result);
}

std::string list::itervalues(const std::string& key, int start, int stop)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("LRANGE");
	result.push_back(key);
	result.push_back(std::to_string(start));
	result.push_back(std::to_string(stop));
	return cmd_join(result);
}
std::string list::values(const std::string& key)
{
	return "LRANGE " + key + " 0 -1";
}
std::string list::trim(const std::string& key, int start, int stop)
{
	std::vector<std::string> result;
	result.reserve(4);
	result.push_back("LTRIM");
	result.push_back(key);
	result.push_back(std::to_string(start));
	result.push_back(std::to_string(stop));
	return cmd_join(result);
}