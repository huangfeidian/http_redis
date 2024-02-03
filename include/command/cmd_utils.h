#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
namespace spiritsaway::http_redis::command
{
	std::string cmd_join(const std::vector<std::string>& fragments);
}
