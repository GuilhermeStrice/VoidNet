#include "Init.hpp"
#include "Config.hpp"

#include <iostream>

bool Initialization::Initialize()
{
	uint16 code = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (code != 0)
	{
		if (Config::GetUsingConsole())
		std::cerr << code << std::endl; // display some more information too
		return false;
	}
	return true;
}

const WSADATA &Initialization::GetData()
{
	return wsa_data;
}