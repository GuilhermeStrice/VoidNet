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

const std::vector<byte> &Utility::BitConverter::FromUint8(uint8 number)
{
	return std::vector<byte>();
}

uint8 Utility::BitConverter::ToUint8(const std::vector<byte> &bytes, uint16 start_index)
{
	return uint8();
}

const std::vector<byte> &Utility::BitConverter::FromUint16(uint16 number)
{
	return std::vector<byte>();
}

uint16 Utility::BitConverter::ToUint16(const std::vector<byte> &bytes, uint16 start_index)
{
	return uint16();
}

const std::vector<byte> & Utility::BitConverter::FromUint32(uint32 number)
{
	return std::vector<byte>();
}

uint32 Utility::BitConverter::ToUint32(const std::vector<byte> & bytes, uint16 start_index)
{
	return uint32();
}

const std::vector<byte> & Utility::BitConverter::FromUint64(uint64 number)
{
	return std::vector<byte>();
}

uint64 Utility::BitConverter::ToUint64(const std::vector<byte> & bytes, uint16 start_index)
{
	return uint64();
}

const std::vector<byte> & Utility::BitConverter::FromInt8(int8 number)
{
	return std::vector<byte>();
}

int8 Utility::BitConverter::ToInt8(const std::vector<byte> & bytes, uint16 start_index)
{
	return int8();
}

const std::vector<byte> & Utility::BitConverter::FromInt16(int16 number)
{
	return std::vector<byte>();
}

int16 Utility::BitConverter::ToInt16(const std::vector<byte> & bytes, uint16 start_index)
{
	return int16();
}

const std::vector<byte> & Utility::BitConverter::FromInt32(int32 number)
{
	return std::vector<byte>();
}

int32 Utility::BitConverter::ToInt32(const std::vector<byte> & bytes, uint16 start_index)
{
	return int32();
}

const std::vector<byte> & Utility::BitConverter::FromInt64(int64 number)
{
	return std::vector<byte>();
}

int64 Utility::BitConverter::ToInt64(const std::vector<byte> & bytes, uint16 start_index)
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

const std::string & Utility::StringConverter::ToString(const std::vector<byte> & bytes, uint16 start_index, uint16 lenght)
{
	// TODO: insert return statement here
}

const void Utility::ConfigReader::ReadConfig(const std::string & file_name)
{
	return void();
}

const std::map<std::string, std::string>& Utility::ConfigReader::ReadNodes()
{
	// TODO: insert return statement here
}

const std::string & Utility::ConfigReader::operator[](uint16 index)
{
	// TODO: insert return statement here
}
