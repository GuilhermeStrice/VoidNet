#include "NetworkMessage.hpp"
#include "Utility.hpp"
#include "Serializer.hpp"
#include "Tags.hpp"

#include <vector>

NetworkMessage::NetworkMessage()
{
}

NetworkMessage::NetworkMessage(uint16 sender, byte distribution_mode, uint16 destination_id, byte tag, uint16 subject, NetworkBuffer buffer) :
	sender(sender), distribution_mode(distribution_mode), destination_id(destination_id), tag(tag), subject(subject), buffer(buffer)
{
}

NetworkMessage::NetworkMessage(const NetworkBuffer &buffer)
{
	*this = DecodeMessage(buffer);
}

NetworkMessage::~NetworkMessage()
{
}

const NetworkBuffer &NetworkMessage::EncodeMessage(const NetworkMessage &message)
{
	return NetworkBuffer();
}

const NetworkMessage &NetworkMessage::DecodeMessage(const NetworkBuffer &buffer)
{
	NetworkMessage message;
	message.sender = Utility::BitConverter::ToUint16(buffer.body, 1);
	message.distribution_mode = buffer.body[3];
	message.destination_id = Utility::BitConverter::ToUint16(buffer.body, 4);
	message.tag = buffer.body[6];
	message.subject = Utility::BitConverter::ToUint16(buffer.body, 7);
	message.buffer = buffer;
	message.valid = message.sender != -2 && message.tag != CONNECT && message.tag != DISCONNECT;

	if (Utility::BitConverter::ToUint32(buffer.header) < 9)
		return message;
	byte version = buffer.body[0];
	switch (version)
	{
	case 0:
	{
		void *object;
		object = Serializer::from_bytes(buffer.body, object);
	}
	default:
	{
		//version not supported
		throw std::runtime_error("NetworkMessage - Decoding version not supported");
	}
	}

	return message;
}