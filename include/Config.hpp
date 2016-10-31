#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Utility.hpp"

struct Config
{
	static void Initialize();
	
	static void SetUsingConsole(bool value);
	static bool GetUsingConsole();

	static void SetLogToFile(bool value);
	static bool GetLogToFile();

	static Utility::ConfigReader Configuration;

private:
	static bool using_console;
	static bool log_to_file;
};

#endif