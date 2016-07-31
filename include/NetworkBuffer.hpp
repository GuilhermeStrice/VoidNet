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

	std::vector<byte> header = Utility::BitConverter::FromUint32(8); // contains the size of the body
	std::vector<byte> body;
};

#endif