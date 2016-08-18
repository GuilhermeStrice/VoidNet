#ifndef HANDSHAKE_HPP
#define HANDSHAKE_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "NetworkMessage.hpp"

#include <vector>

struct Handshake
{
	Handshake();
	Handshake(uint16 id, byte con_code, byte distribution_mode);
	~Handshake();

	static const std::vector<byte> &EncodeHandshake(const Handshake &handshake);
	static Handshake &DecodeHandshake(const std::vector<byte> &bytes);
	static const NetworkMessage &HandshakeToNetworkMessage(const Handshake &handshake);
	static const Handshake &NetworkMessageToHandshake(const NetworkMessage &message);

	uint16 id;
	byte con_code;
	byte distribution_mode;
};

#endif
