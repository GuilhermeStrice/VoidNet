#ifndef INIT_HPP
#define INIT_HPP

#ifdef _MSC_VER
	#pragma once
#endif

#include "Defs.hpp"


struct Initialization
{
	static bool Initialize();

#ifdef _MSC_VER
	const WSADATA &GetData();
#endif

private:
#ifdef _MSC_VER
	static WSADATA wsa_data;
#endif
};

#endif