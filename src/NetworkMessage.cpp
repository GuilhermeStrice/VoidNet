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
	NetworkBuffer net_buffer;

	std::vector<byte> sender = Utility::BitConverter::FromUint16(message.sender);
	std::vector<byte> distribution_mode = Utility::BitConverter::FromUint8(message.distribution_mode);
	std::vector<byte> destination_id = Utility::BitConverter::FromUint16(message.destination_id);
	std::vector<byte> tag = Utility::BitConverter::FromUint8(message.tag);
	std::vector<byte> subject = Utility::BitConverter::FromUint16(message.subject);
	std::vector<byte> data;
	if (message.data != nullptr)
		data = Serializer::to_bytes(message.data);

	net_buffer.body.insert(net_buffer.body.end(), sender.begin(), sender.end());
	net_buffer.body.insert(net_buffer.body.end(), distribution_mode.begin(), distribution_mode.end());
	net_buffer.body.insert(net_buffer.body.end(), destination_id.begin(), destination_id.end());
	net_buffer.body.insert(net_buffer.body.end(), tag.begin(), tag.end());
	net_buffer.body.insert(net_buffer.body.end(), subject.begin(), subject.end());
	if (message.data != nullptr && data.size() > 0)
		net_buffer.body.insert(net_buffer.body.end(), data.begin(), data.end());
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