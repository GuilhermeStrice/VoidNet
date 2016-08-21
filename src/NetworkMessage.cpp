#include "NetworkMessage.hpp"
#include "Utility.hpp"
#include "Serializer.hpp"
#include "Handshake.hpp"

#include <vector>

NetworkMessage::NetworkMessage()
{
}

NetworkMessage::NetworkMessage(uint16 sender, byte distribution_mode, uint16 destination_id, byte tag, byte subject) :
	sender(sender), distribution_mode(distribution_mode), destination_id(destination_id), tag(tag), subject(subject)
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
	if (message.valid)
	{
		NetworkBuffer net_buffer;

		if (!IS_HANDSHAKE(message))
		{
			std::vector<byte> sender = Utility::BitConverter::ToBytes(message.sender);
			std::vector<byte> distribution_mode = Utility::BitConverter::ToBytes(message.distribution_mode);
			std::vector<byte> destination_id = Utility::BitConverter::ToBytes(message.destination_id);
			std::vector<byte> tag = Utility::BitConverter::ToBytes(message.tag);
			std::vector<byte> subject = Utility::BitConverter::ToBytes(message.subject);
			std::vector<byte> data;
			if (message.data != nullptr)
				data = Serializer::to_bytes(message.data);
			std::vector<byte> type = Utility::BitConverter::ToBytes(0);

			net_buffer.body.insert(net_buffer.body.begin(), type.begin(), type.end());
			net_buffer.body.insert(net_buffer.body.begin(), sender.begin(), sender.end());
			net_buffer.body.insert(net_buffer.body.begin(), distribution_mode.begin(), distribution_mode.end());
			net_buffer.body.insert(net_buffer.body.begin(), destination_id.begin(), destination_id.end());
			net_buffer.body.insert(net_buffer.body.begin(), tag.begin(), tag.end());
			net_buffer.body.insert(net_buffer.body.begin(), subject.begin(), subject.end());
			if (message.data != nullptr && data.size() > 0)
				net_buffer.body.insert(net_buffer.body.begin(), data.begin(), data.end());
			net_buffer.header = Utility::BitConverter::ToBytes(sender.size() + distribution_mode.size() + destination_id.size() +
				tag.size() + subject.size() + data.size());
			net_buffer.valid = true;
		}
		else
		{
			std::vector<byte> handshake_bytes = Handshake::EncodeHandshake(Handshake::NetworkMessageToHandshake(message));
			std::vector<byte> type = Utility::BitConverter::ToBytes(static_cast<uint8>(1));
			handshake_bytes.insert(handshake_bytes.begin(), type.begin(), type.end());
			net_buffer.header = Utility::BitConverter::ToBytes(handshake_bytes.size());
			net_buffer.body = handshake_bytes;
		}
		return net_buffer;
	}
	return NetworkBuffer();
}

const NetworkMessage &NetworkMessage::DecodeMessage(const NetworkBuffer &buffer)
{
	if (buffer.valid)
	{
		byte type = buffer.body[0];
		switch (type)
		{
		case 0:
		{
			NetworkMessage message;
			message.sender = Utility::BitConverter::ToUint16(buffer.body, 1);
			message.distribution_mode = buffer.body[3];
			message.destination_id = Utility::BitConverter::ToUint16(buffer.body, 4);
			message.tag = buffer.body[6];
			message.subject = Utility::BitConverter::ToUint8(buffer.body, 7);
			message.valid = message.sender != -2 && message.tag != ConnectTag && message.tag != DisconnectTag;
			if (Utility::BitConverter::ToInt32(buffer.header) < 9)
				return message;
			void *object;
			object = Serializer::from_bytes(buffer.body, object);
			return message;
		}
		case 1:
		{
			NetworkMessage message;
			message.sender = Utility::BitConverter::ToUint16(buffer.body, 1);
			message.tag = Utility::BitConverter::ToUint8(buffer.body, 3);
			message.subject = 1;
			message.valid = message.sender != -2 && IS_HANDSHAKE(message);
			return message;
			break;
		}
		default:
		{
			//type not supported
			throw std::runtime_error("NetworkMessage - Decoding version not supported");
		}
		}

	}
	return NetworkMessage();
}