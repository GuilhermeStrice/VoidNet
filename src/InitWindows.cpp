#include "Init.hpp"
#include "Config.hpp"

#include <iostream>

VoidCode Initialization::initialize()
{
	uint16 code = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (code != 0)
	{
		if (Config::GetUsingConsole())
		std::cerr << code << std::endl; // display some more information too
		return VOID_WSA_INIT_FAILED;
	}
	return VOID_SUCCESS;
}