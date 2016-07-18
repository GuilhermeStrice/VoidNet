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
	NetworkMessage(uint16 sender, byte distribution_mode, uint16 destination_id, byte tag, uint16 subject, NetworkBuffer buffer);
	~NetworkMessage();

	static const NetworkBuffer &EncodeMessage(const NetworkMessage &message);
	
	const NetworkMessage &NetworkMessage::DecodeMessage(const NetworkBuffer &buffer);

	uint16 sender = -2;
	byte distribution_mode;
	uint16 destination_id;
	byte tag;
	uint16 subject;
	void *data;
	
	bool valid = false;

private:
	const NetworkMessage &NetworkMessage::DecodeMessageHeader(const NetworkBuffer &buffer);
	void *DecodeMessageData(const NetworkBuffer &buffer);
	NetworkBuffer buffer;
};

#endif
