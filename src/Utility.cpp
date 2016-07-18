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
	
}

uint8 Utility::BitConverter::ToUint8(byte *bytes, uint16 start_index)
{
	
}

byte *Utility::BitConverter::FromUint16(uint16 number)
{
	
}

uint16 Utility::BitConverter::ToUint16(byte *bytes, uint16 start_index)
{
	
}

byte * Utility::BitConverter::FromUint32(uint32 number)
{
	return nullptr;
}

uint32 * Utility::BitConverter::ToUint32(byte * bytes, uint16 start_index)
{
	return nullptr;
}

byte * Utility::BitConverter::FromUint64(uint64 number)
{
	return nullptr;
}

uint64 Utility::BitConverter::ToUint64(byte * bytes, uint16 start_index)
{
	return uint64();
}

byte * Utility::BitConverter::FromInt8(int8 number)
{
	return nullptr;
}

int8 Utility::BitConverter::ToInt8(byte * bytes, uint16 start_index)
{
	return int8();
}

byte * Utility::BitConverter::FromInt16(int16 number)
{
	return nullptr;
}

int16 Utility::BitConverter::ToInt16(byte * bytes, uint16 start_index)
{
	return int16();
}

byte * Utility::BitConverter::FromInt32(int32 number)
{
	return nullptr;
}

int32 Utility::BitConverter::ToInt32(byte * bytes, uint16 start_index)
{
	return int32();
}

byte * Utility::BitConverter::FromInt64(int64 number)
{
	return nullptr;
}

int64 Utility::BitConverter::ToInt64(byte * bytes, uint16 start_index)
{
	return int64();
}

const std::string & Utility::StringConverter::ToString(bool value)
{
	// TODO: insert return statement here
}

const std::string & Utility::StringConverter::ToString(uint8 value)
{
	// TODO: insert return statement here
}

const std::string & Utility::StringConverter::ToString(uint16 value)
{
	// TODO: insert return statement here
}

const std::string & Utility::StringConverter::ToString(uint32 value)
{
	// TODO: insert return statement here
}

const std::string & Utility::StringConverter::ToString(uint64 value)
{
	// TODO: insert return statement here
}

const std::string & Utility::StringConverter::ToString(int8 value)
{
	// TODO: insert return statement here
}

const std::string & Utility::StringConverter::ToString(int16 value)
{
	// TODO: insert return statement here
}

const std::string & Utility::StringConverter::ToString(int32 value)
{
	// TODO: insert return statement here
}

const std::string & Utility::StringConverter::ToString(int64 value)
{
	// TODO: insert return statement here
}

const std::string & Utility::StringConverter::ToString(byte * bytes, uint16 start_index, uint16 lenght)
{
	// TODO: insert return statement here
}
