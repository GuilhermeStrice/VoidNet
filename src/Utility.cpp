#include "Utility.hpp"

void Utility::Delete(void *pointer)
{
	if (pointer == nullptr)
		return;
	delete pointer;
	pointer = nullptr;
}

void Utility::DeleteArray(void *pointer)
{
	if (pointer == nullptr)
		return;
	delete[] pointer;
	pointer = nullptr;
}

byte *Utility::BitConverter::FromUint8(uint8 number)
{
	if (number == 0)
		return nullptr;
	return reinterpret_cast<byte*>(number);
}

uint8 Utility::BitConverter::ToUint8(byte *bytes, uint16 start_index)
{
	if (bytes == nullptr)
		return 0;
	return reinterpret_cast<uint8>(&bytes[start_index]);
}

byte *Utility::BitConverter::FromUint16(uint16 number)
{
	if (number == 0)
		return nullptr;
	return reinterpret_cast<byte*>(number);
}

uint16 Utility::BitConverter::ToUint16(byte *bytes, uint16 start_index)
{
	if (bytes == nullptr)
		return 0;
	return reinterpret_cast<uint16>(&bytes[start_index]);
}
