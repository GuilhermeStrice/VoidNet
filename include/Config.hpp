#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifdef _MSC_VER
#pragma once
#endif

struct Config
{
	static void SetUsingConsole(bool value);
	static bool GetUsingConsole();

	static void SetLogToFile(bool value);
	static bool GetLogToFile();

private:
	static bool using_console;
	static bool log_to_file;
};

#endif