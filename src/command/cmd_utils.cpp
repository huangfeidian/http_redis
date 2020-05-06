#include <command/cmd_utils.h>

using namespace spiritsaway::http_redis::command;
std::string cmd_join(const std::vector<std::string>& fragments)
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