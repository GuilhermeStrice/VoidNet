#include "Config.hpp"

inline void Config::SetUsingConsole(bool value)
{
	using_console = value;
}

inline bool Config::GetUsingConsole()
{
	return using_console;
}

inline void Config::SetLogToFile(bool value)
{
	log_to_file = value;
}

inline bool Config::GetLogToFile()
{
	return log_to_file;
}
