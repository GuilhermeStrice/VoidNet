#ifndef NETWORK_BUFFER_HPP
#define NETWORK_BUFFER_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "Utility.hpp"

#include <vector>

struct NetworkBuffer
{
	NetworkBuffer();
	~NetworkBuffer();

	std::vector<byte> header; // size must always be 8
	std::vector<byte> body;

	bool valid = false;
};

#endif