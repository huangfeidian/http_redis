#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <nlohmann/json.hpp>

namespace spiritsaway::http_redis::command
{
	std::string cmd_join(const std::vector<std::string>& fragments);
	std::string to_redis_str(const nlohmann::json& input);
}
