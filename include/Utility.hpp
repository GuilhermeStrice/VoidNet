#ifndef UTILITY_HPP
#define UTILITY_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"

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
	};
};

#endif