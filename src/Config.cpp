#include "Config.hpp"

void Config::SetUsingConsole(bool value)
{
	using_console = value;
}

bool Config::GetUsingConsole()
{
	return using_console;
}
