#pragma once

// i want to use bitshifting but i red in stackoverflow that casting doesnt generate overhead
// now ive hit a wall
// casting vs bitshifting

#include "VoidNet_LL/Net.hpp"

namespace std
{
	class ByteConverter
	{
	public:
		template<typename T>
		inline static byte *ToBytes(T value)
		{
			byte *data = new byte[sizeof(T)]();
			memcpy(data, &value, sizeof(T));
			return data;
		}

		template<typename T>
		inline static T FromBytes(byte *data)
		{
			if (!data)
				throw std::invalid_argument("cant have null parameter -> ByteConverter::FromBytes");
			T value;
			memcpy(&value, data, sizeof(T));
			return value;
		}
	};
}