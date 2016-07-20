#ifndef INIT_HPP
#define INIT_HPP

#ifdef _MSC_VER
	#pragma once
#endif

#include "Defs.hpp"


struct Initialization
{
	static bool initialize();
	static WSADATA wsa_data;
};

#endif