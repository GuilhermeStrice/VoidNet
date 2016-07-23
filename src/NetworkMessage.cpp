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
	NetworkBuffer buffer;
	uint16 size = sizeof(buffer.body_size) + sizeof(buffer.body);
	byte *encoded_message = new byte[size]();
	buffer.body_size = size;

	byte *encoded_sender = &encoded_message[1];
	encoded_sender = Utility::BitConverter::FromUint16(message.sender);

	encoded_message[3] = message.distribution_mode;

	byte *encoded_destination_id = &encoded_message[4];
	encoded_destination_id = Utility::BitConverter::FromUint16(message.destination_id);

	encoded_message[6] = message.tag;

	byte *encoded_subject = &encoded_message[7];
	encoded_subject = Utility::BitConverter::FromUint16(message.subject);

	byte *encoded_data = &encoded_message[9];
	std::vector<byte> serialized_data = Serializer::to_bytes(message.data);
	encoded_data = &serialized_data[0];

	buffer.body = encoded_message;
	return buffer;
}

const NetworkMessage &NetworkMessage::DecodeMessage(const NetworkBuffer &buffer)
{
	NetworkMessage message = decode_message_header(buffer);
	message.data = decode_message_data(buffer);
	return message;
}

void *NetworkMessage::decode_message_data(const NetworkBuffer &buffer)
{
	if (buffer.body_size < 9)
		return nullptr;
	byte version = buffer.body[0];
	switch (version)
	{
	case 0:
	{
		void *object;
		return Serializer::from_bytes(buffer.body + 9, object);
	}
	default:
	{
		//version nor supported
		throw std::runtime_error("NetworkMessage - Decoding version not supported");
	}
	}
}

const NetworkMessage &NetworkMessage::decode_message_header(const NetworkBuffer &buffer)
{
	sender = Utility::BitConverter::ToUint16(buffer.body, 1);
	distribution_mode = buffer.body[3];
	destination_id = Utility::BitConverter::ToUint16(buffer.body, 4);
	tag = buffer.body[6];
	subject = Utility::BitConverter::ToUint16(buffer.body, 7);
	this->buffer = buffer;
	valid = sender != -2 && tag != CONNECT && tag != DISCONNECT && distribution_mode > 0;
	return *this;
}
