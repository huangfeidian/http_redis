#pragma once
#include <string>
#include <vector>
#include <sstream>
namespace spiritsaway::http_redis::command
{
	std::string cmd_join(const std::vector<std::string>& fragments);
}
