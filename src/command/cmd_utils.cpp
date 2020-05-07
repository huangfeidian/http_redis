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