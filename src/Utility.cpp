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
