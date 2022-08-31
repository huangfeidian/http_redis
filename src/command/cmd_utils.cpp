#include <command/cmd_utils.h>

std::string spiritsaway::http_redis::command::cmd_join(const std::vector<std::string>& fragments)
{
	std::stringstream ss;
	for(size_t i = 0; i < fragments.size(); ++i)
	{
	if(i != 0)
		ss << " ";
	ss << fragments[i];
	}
	return ss.str();
}

std::string spiritsaway::http_redis::command::to_redis_str(const nlohmann::json& input)
{
	std::string temp_result = input.dump();
	std::string final_result;
	final_result.reserve(std::uint32_t(temp_result.size() * 1.2));
	final_result.push_back('\'');
	for(auto a: temp_result)
	{
		if(a == '\'')
		{
			final_result.push_back('\\');
			final_result.push_back('\'');
		}
		else
		{
			final_result.push_back(a);
		}
	}
	final_result.push_back('\'');
	return final_result;
}