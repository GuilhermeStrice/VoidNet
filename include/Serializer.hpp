#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"

#include <array>

class Serializer
{
	template<typename T> std::array<byte, sizeof(T)> to_bytes(const T& object);
	template<typename T> T& from_bytes(const std::array<byte, sizeof(T)> &bytes, T& object);
};

#endif