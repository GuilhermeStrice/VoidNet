#include "NetworkMessage.hpp"
#include "Utility.hpp"
#include "Serializer.hpp"
#include <vector>

NetworkMessage::NetworkMessage()
{
}

NetworkMessage::NetworkMessage(uint16 sender, byte distribution_mode, uint16 destination_id, byte tag, uint16 subject, NetworkBuffer buffer) :
	sender(sender), distribution_mode(distribution_mode), destination_id(destination_id), tag(tag), subject(subject), buffer(buffer)
{
}

NetworkMessage::~NetworkMessage()
{
}

const NetworkMessage &NetworkMessage::EncodeMessage()
{
	return NetworkMessage();
}

const NetworkMessage &NetworkMessage::DecodeMessage(const NetworkBuffer &buffer)
{
	NetworkMessage message = DecodeMessageHeader(buffer);
	message.data = DecodeMessageData(buffer);
	return message;
}

void *NetworkMessage::DecodeMessageData(const NetworkBuffer &buffer)
{
	if (buffer.body_size < 9)
		return nullptr;
	byte version = buffer.body[0];
	switch (version)
	{
	case 0:
	{
		void *object;
		return Serializer::from_bytes(buffer.body, object);
	}
	default:
	{
		//version nor supported
		throw std::runtime_error("NetworkMessage - Decoding version not supported");
	}
	}
}

const NetworkMessage &NetworkMessage::DecodeMessageHeader(const NetworkBuffer &buffer)
{
	return NetworkMessage(Utility::BitConverter::ToUint16(buffer.body, 1), buffer.body[3], Utility::BitConverter::ToUint16(buffer.body, 4),
		buffer.body[6], Utility::BitConverter::ToUint16(buffer.body, 7), buffer);
}
