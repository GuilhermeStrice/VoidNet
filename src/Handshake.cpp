#include "Handshake.hpp"
#include "Utility.hpp"

Handshake::Handshake()
{
	id = -2;
}

Handshake::Handshake(uint16 id, byte con_code, byte distribution_mode)
{
	this->id = id;
	this->con_code = con_code;
	this->distribution_mode = distribution_mode;
}

Handshake::~Handshake()
{
}

const std::vector<byte>& Handshake::EncodeHandshake(const Handshake & handshake)
{
	std::vector<byte> handshake_bytes;

	std::vector<byte> id = Utility::BitConverter::ToBytes(handshake.id);
	std::vector<byte> con_mode = Utility::BitConverter::ToBytes(handshake.con_code);
	std::vector<byte> type = Utility::BitConverter::ToBytes(static_cast<uint8>(1));

	handshake_bytes.insert(handshake_bytes.begin(), type.begin(), type.end());
	handshake_bytes.insert(handshake_bytes.begin(), id.begin(), id.end());
	handshake_bytes.insert(handshake_bytes.begin(), con_mode.begin(), con_mode.end());

	return handshake_bytes;
}

Handshake & Handshake::DecodeHandshake(const std::vector<byte>& bytes)
{
	Handshake handshake;
	
	handshake.id = Utility::BitConverter::ToUint16(bytes, 1);
	handshake.con_code = Utility::BitConverter::ToUint8(bytes, 3);

	return handshake;
}

const NetworkMessage & Handshake::HandshakeToNetworkMessage(const Handshake & handshake)
{
	NetworkMessage message;
	message.sender = handshake.id;
	message.tag = handshake.con_code;
	message.subject = 1;
	return message;
}

const Handshake & Handshake::NetworkMessageToHandshake(const NetworkMessage & message)
{
	Handshake handshake;
	handshake.id = message.sender;
	handshake.con_code = message.tag;
	return handshake;
}
