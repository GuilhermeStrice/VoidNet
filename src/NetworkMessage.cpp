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

		if (message.subject != 1)
		{
			std::vector<byte> sender = Utility::BitConverter::FromUint16(message.sender);
			std::vector<byte> distribution_mode = Utility::BitConverter::FromUint8(message.distribution_mode);
			std::vector<byte> destination_id = Utility::BitConverter::FromUint16(message.destination_id);
			std::vector<byte> tag = Utility::BitConverter::FromUint8(message.tag);
			std::vector<byte> subject = Utility::BitConverter::FromUint16(message.subject);
			std::vector<byte> data;
			if (message.data != nullptr)
				data = Serializer::to_bytes(message.data);

			net_buffer.body.emplace_back(Utility::BitConverter::FromInt8(0));
			net_buffer.body.emplace_back(sender.begin(), sender.end());
			net_buffer.body.emplace_back(distribution_mode.begin(), distribution_mode.end());
			net_buffer.body.emplace_back(destination_id.begin(), destination_id.end());
			net_buffer.body.emplace_back(tag.begin(), tag.end());
			net_buffer.body.emplace_back(subject.begin(), subject.end());
			if (message.data != nullptr && data.size() > 0)
				net_buffer.body.emplace_back(data.begin(), data.end());
			net_buffer.header = Utility::BitConverter::FromInt32(sender.size() + distribution_mode.size() + destination_id.size() +
				tag.size() + subject.size() + data.size());
			net_buffer.valid = true;
		}
		else
		{
			std::vector<byte> handshake_bytes = Handshake::EncodeHandshake(Handshake::NetworkMessageToHandshake(message));
			net_buffer.header = Utility::BitConverter::FromInt32(handshake_bytes.size());
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
			message.valid = message.sender != -2 && message.tag != CONNECT && message.tag != DISCONNECT;
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
			message.valid = message.sender != -2 && (message.tag == DISCONNECT || message.tag == CONNECT || message.tag == ConnectionCode::Accept ||
				message.tag == ConnectionCode::Close || message.tag == ConnectionCode::Reject);
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