#ifndef HANDSHAKE_HPP
#define HANDSHAKE_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"

#include <vector>

struct Handshake
{
	Handshake();
	Handshake(uint16 id, byte con_code);
	~Handshake();

	static const std::vector<byte> &EncodeHandshake(const Handshake &handshake);
	static Handshake &DecodeHandshake(const std::vector<byte> &bytes);

	uint16 id;
	byte con_code;
};

#endif
