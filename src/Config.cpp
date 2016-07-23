#include "Config.hpp"

void Config::SetUsingConsole(bool value)
{
	using_console = value;
}

bool Config::GetUsingConsole()
{
	return using_console;
}

void Config::SetLogToFile(bool value)
{
	log_to_file = value;
}

bool Config::GetLogToFile()
{
	return log_to_file;
}
