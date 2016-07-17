#ifndef NETWORK_BUFFER_HPP
#define NETWORK_BUFFER_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"

struct NetworkBuffer
{
	NetworkBuffer();
	~NetworkBuffer();

	uint32 body_size;
	byte *body = nullptr;
};

#endif