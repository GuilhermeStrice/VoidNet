#ifndef UTILITY_HPP
#define UTILITY_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"

#include <string>
#include <map>
#include <vector>

struct Utility
{
	static void Delete(void *pointer);
	static void DeleteArray(void *pointer);

	struct BitConverter
	{
		static const std::vector<byte> &ToBytes(uint8 number);
		static uint8 ToUint8(const std::vector<byte> &bytes, uint16 start_index = 0);

		static const std::vector<byte> &ToBytes(uint16 number);
		static uint16 ToUint16(const std::vector<byte> &bytes, uint16 start_index = 0);

		static const std::vector<byte> &ToBytes(uint32 number);
		static uint32 ToUint32(const std::vector<byte> &bytes, uint16 start_index = 0);
		
		static const std::vector<byte> &ToBytes(uint64 number);
		static uint64 ToUint64(const std::vector<byte> &bytes, uint16 start_index = 0);

		static const std::vector<byte> &ToBytes(int8 number);
		static int8 ToInt8(const std::vector<byte> &bytes, uint16 start_index = 0);

		static const std::vector<byte> &ToBytes(int16 number);
		static int16 ToInt16(const std::vector<byte> &bytes, uint16 start_index = 0);

		static const std::vector<byte> &ToBytes(int32 number);
		static int32 ToInt32(const std::vector<byte> &bytes, uint16 start_index = 0);

		static const std::vector<byte> &ToBytes(int64 number);
		static int64 ToInt64(const std::vector<byte> &bytes, uint16 start_index = 0);
	};

	struct StringConverter
	{
		static const std::string &ToString(bool value);
		static const std::string &ToString(uint8 value);
		static const std::string &ToString(uint16 value);
		static const std::string &ToString(uint32 value);
		static const std::string &ToString(uint64 value);
		static const std::string &ToString(int8 value);
		static const std::string &ToString(int16 value);
		static const std::string &ToString(int32 value);
		static const std::string &ToString(int64 value);
		static const std::string &ToString(const std::vector<byte> &bytes);

		static uint8 ToUint8(const std::string &str);
		static uint16 ToUint16(const std::string &str);
		static uint32 ToUint32(const std::string &str);
		static uint64 ToUint64(const std::string &str);
		static int8 ToInt8(const std::string &str);
		static int16 ToInt16(const std::string &str);
		static int32 ToInt32(const std::string &str);
		static int64 ToInt64(const std::string &str);

		static const std::vector<byte> &ToBytes(const std::string &str);

		static const std::string &ToString(const std::vector<byte> &bytes, uint16 start_index = 0, uint16 lenght = 0);

		static const std::string &Trim(std::string &str, char ch);
		static std::vector<std::string> Split(const std::string &str, const std::string &delimiter);
	};

	struct IPUtil
	{
		static bool ValidIPV4(const std::string &ip);
	};

	struct ConfigReader
	{
		void ReadConfig(const std::string &file_name);
		void ReadNodes();

		const std::string &operator[](const std::string &key);

	private:
		std::map<std::string, std::string> nodes;
		std::string file_content;
	};
};

#endif