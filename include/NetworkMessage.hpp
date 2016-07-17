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

	const NetworkMessage &EncodeMessage();
	
	const NetworkMessage &NetworkMessage::DecodeMessage(const NetworkBuffer &buffer);
	const NetworkMessage &NetworkMessage::DecodeMessageHeader(const NetworkBuffer &buffer);
	void *DecodeMessageData(const NetworkBuffer &buffer);

	uint16 sender;
	uint16 destination_id;
	byte distribution_mode;
	uint16 tag;
	uint16 subject;
	void *data;

private:
	NetworkBuffer buffer;
};

#endif
