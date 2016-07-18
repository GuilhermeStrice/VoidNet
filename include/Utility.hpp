#ifndef UTILITY_HPP
#define UTILITY_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"

#include <string>
#include <streambuf>
#include <istream>

struct Utility
{
	static void Delete(void *pointer);
	static void DeleteArray(void *pointer);

	struct BitConverter
	{
		static byte *FromUint8(uint8 number);
		static uint8 ToUint8(byte *bytes, uint16 start_index);

		static byte *FromUint16(uint16 number);
		static uint16 ToUint16(byte *bytes, uint16 start_index);

		static byte *FromUint32(uint32 number);
		static uint32 *ToUint32(byte *bytes, uint16 start_index);
		
		static byte *FromUint64(uint64 number);
		static uint64 ToUint64(byte *bytes, uint16 start_index);

		static byte *FromInt8(int8 number);
		static int8 ToInt8(byte *bytes, uint16 start_index);

		static byte *FromInt16(int16 number);
		static int16 ToInt16(byte *bytes, uint16 start_index);

		static byte *FromInt32(int32 number);
		static int32 ToInt32(byte *bytes, uint16 start_index);

		static byte *FromInt64(int64 number);
		static int64 ToInt64(byte *bytes, uint16 start_index);
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

		static const std::string &ToString(byte *bytes, uint16 start_index = 0, uint16 lenght = 0);
	};
};

#endif