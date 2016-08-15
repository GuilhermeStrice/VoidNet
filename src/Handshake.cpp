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

	std::vector<byte> id = Utility::BitConverter::FromUint16(handshake.id);
	std::vector<byte> con_mode = Utility::BitConverter::FromUint8(handshake.con_code);
	
	handshake_bytes.insert(handshake_bytes.end(), id.begin(), id.end());
	handshake_bytes.insert(handshake_bytes.end(), con_mode.begin(), con_mode.end());

	return handshake_bytes;
}

Handshake & Handshake::DecodeHandshake(const std::vector<byte>& bytes)
{
	Handshake handshake;
	
	handshake.id = Utility::BitConverter::ToUint16(bytes);
	handshake.con_code = Utility::BitConverter::ToUint8(bytes, 2);

	return handshake;
}
