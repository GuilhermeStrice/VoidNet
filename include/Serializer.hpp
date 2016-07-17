#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"

struct Serializer
{
	template<typename T> static const std::vector<byte> &to_bytes(const T &object);
	template<typename T> static const T &from_bytes(byte *bytes, T &object);
};

#endif