#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifdef _MSC_VER
#pragma once
#endif

struct Config
{
	static void SetUsingConsole(bool value);
	static bool GetUsingConsole();

private:
	static bool using_console;
};

#endif