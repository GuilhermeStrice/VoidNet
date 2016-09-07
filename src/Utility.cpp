#include "Utility.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

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

std::vector<std::string> Utility::StringConverter::Split(const std::string & str, const std::string & delimiter)
{
	std::vector<std::string> splited;
	if (str.empty() && delimiter.empty())
		return std::vector<std::string>();
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter[0]))
		splited.push_back(token);
	return splited;
}

const std::vector<byte> &Utility::BitConverter::ToBytes(uint8 number)
{
	return std::vector<byte>();
}

uint8 Utility::BitConverter::ToUint8(const std::vector<byte> &bytes, uint16 start_index)
{
	return uint8();
}

const std::vector<byte> &Utility::BitConverter::ToBytes(uint16 number)
{
	return std::vector<byte>();
}

uint16 Utility::BitConverter::ToUint16(const std::vector<byte> &bytes, uint16 start_index)
{
	return uint16();
}

const std::vector<byte> & Utility::BitConverter::ToBytes(uint32 number)
{
	return std::vector<byte>();
}

uint32 Utility::BitConverter::ToUint32(const std::vector<byte> & bytes, uint16 start_index)
{
	return uint32();
}

const std::vector<byte> & Utility::BitConverter::ToBytes(uint64 number)
{
	return std::vector<byte>();
}

uint64 Utility::BitConverter::ToUint64(const std::vector<byte> & bytes, uint16 start_index)
{
	return uint64();
}

const std::vector<byte> & Utility::BitConverter::ToBytes(int8 number)
{
	return std::vector<byte>();
}

int8 Utility::BitConverter::ToInt8(const std::vector<byte> & bytes, uint16 start_index)
{
	return int8();
}

const std::vector<byte> & Utility::BitConverter::ToBytes(int16 number)
{
	return std::vector<byte>();
}

int16 Utility::BitConverter::ToInt16(const std::vector<byte> & bytes, uint16 start_index)
{
	return int16();
}

const std::vector<byte> & Utility::BitConverter::ToBytes(int32 number)
{
	return std::vector<byte>();
}

int32 Utility::BitConverter::ToInt32(const std::vector<byte> & bytes, uint16 start_index)
{
	return int32();
}

const std::vector<byte> & Utility::BitConverter::ToBytes(int64 number)
{
	return std::vector<byte>();
}

int64 Utility::BitConverter::ToInt64(const std::vector<byte> & bytes, uint16 start_index)
{
	return int64();
}

const std::string & Utility::StringConverter::ToString(bool value)
{
	return std::string();
}

const std::string & Utility::StringConverter::ToString(uint8 value)
{
	return std::string();
}

const std::string & Utility::StringConverter::ToString(uint16 value)
{
	return std::string();
}

const std::string & Utility::StringConverter::ToString(uint32 value)
{
	return std::string();
}

const std::string & Utility::StringConverter::ToString(uint64 value)
{
	return std::string();
}

const std::string & Utility::StringConverter::ToString(int8 value)
{
	return std::string();
}

const std::string & Utility::StringConverter::ToString(int16 value)
{
	return std::string();
}

const std::string & Utility::StringConverter::ToString(int32 value)
{
	return std::string();
}

const std::string & Utility::StringConverter::ToString(int64 value)
{
	return std::string();
}

const std::string & Utility::StringConverter::ToString(const std::vector<byte>& bytes)
{
	return std::string();
}

uint8 Utility::StringConverter::ToUint8(const std::string & str)
{
	return uint8();
}

uint16 Utility::StringConverter::ToUint16(const std::string & str)
{
	return uint16();
}

uint32 Utility::StringConverter::ToUint32(const std::string & str)
{
	return uint32();
}

uint64 Utility::StringConverter::ToUint64(const std::string & str)
{
	return uint64();
}

int8 Utility::StringConverter::ToInt8(const std::string & str)
{
	return int8();
}

int16 Utility::StringConverter::ToInt16(const std::string & str)
{
	return int16();
}

int32 Utility::StringConverter::ToInt32(const std::string & str)
{
	return int32();
}

int64 Utility::StringConverter::ToInt64(const std::string & str)
{
	return int64();
}

const std::vector<byte>& Utility::StringConverter::ToBytes(const std::string & str)
{
	return std::vector<byte>();
}

const std::string & Utility::StringConverter::ToString(const std::vector<byte> & bytes, uint16 start_index, uint16 lenght)
{
	return std::string();
}

const std::string & Utility::StringConverter::Trim(std::string & str, char ch)
{
	if (str.empty() && ch == 0)
		return std::string();
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (*it == ch)
			str.erase(it);
	}
	return str;
}

void Utility::ConfigReader::ReadConfig(const std::string & file_name)
{
	if (file_name.empty())
		return;
	std::fstream file;
	file.open(file_name);
	if (file.is_open())
	{
		longlong file_lenght = file.gcount();
		char *content = new char[static_cast<uint32>(file_lenght)]();
		file.read(content, file_lenght);
		file_content = std::string(content);
	}
}

void Utility::ConfigReader::ReadNodes()
{
	if (file_content.empty())
		return;
	std::stringstream ss(file_content);
	std::string temp;
	std::vector<std::string> nodes_lines;
	while (std::getline(ss, temp, '\n'))
	{
		if (temp.substr(0, 2) != "//")
			nodes_lines.emplace_back(temp);
	}

	for (std::vector<std::string>::iterator it = nodes_lines.begin(); it != nodes_lines.end(); ++it)
	{
		std::string node_str = Utility::StringConverter::Trim(*it, ' ');
		std::vector<std::string> node = Utility::StringConverter::Split(node_str, "=");
		nodes.insert(std::pair<std::string, std::string>(node.at(0), node.at(1)));
	}
}

const std::string & Utility::ConfigReader::operator[](const std::string &key)
{
	return nodes.at(key);
}

bool Utility::IPUtil::ValidIPV4(const std::string & ip)
{
	std::vector<std::string> splitted_address = Utility::StringConverter::Split(ip, ".");
	if (splitted_address.size() != 4)
		return false;
	uint8 a1 = Utility::StringConverter::ToUint8(splitted_address[0]);
	uint8 a2 = Utility::StringConverter::ToUint8(splitted_address[1]);
	uint8 a3 = Utility::StringConverter::ToUint8(splitted_address[2]);
	uint8 a4 = Utility::StringConverter::ToUint8(splitted_address[3]);

	return a1 != 0 && a2 != 0 && a3 != 0 && a4 != 0 &&
		a1 != 255 && a2 != 255 && a3 != 255 && a4 != 255;
}
