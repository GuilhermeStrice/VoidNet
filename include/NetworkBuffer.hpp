#ifndef NETWORK_BUFFER_HPP
#define NETWORK_BUFFER_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"

#include <vector>

struct NetworkBuffer
{
	NetworkBuffer();
	~NetworkBuffer();

	std::vector<byte> header; // contains the size of the body
	std::vector<byte> body;
};

#endif