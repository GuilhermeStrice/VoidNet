#ifndef NETWORK_MESSAGE_HPP
#define NETWORK_MESSAGE_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "NetworkBuffer.hpp"

struct NetworkMessage
{
	NetworkMessage();
	NetworkMessage(const NetworkBuffer &buffer);
	NetworkMessage(uint16 sender, byte distribution_mode, uint16 destination_id, byte tag, byte subject);
	~NetworkMessage();

	static const NetworkBuffer &EncodeMessage(const NetworkMessage &message);
	
	static const NetworkMessage &DecodeMessage(const NetworkBuffer &buffer);

	uint16 sender = -2;
	byte distribution_mode;
	uint16 destination_id;
	byte tag;
	byte subject;
	void *data;
	
	bool valid = false;
};

#endif
