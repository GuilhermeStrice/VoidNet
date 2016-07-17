#include "Serializer.hpp"

#include <vector>

template<typename T> const std::vector<byte> &Serializer::to_bytes(const T& object)
{
	std::vector<byte> bytes;

	const byte *begin = reinterpret_cast<const byte*>(std::addressof(object));
	const byte *end = begin + sizeof(T);
	std::copy(begin, end, std::begin(bytes));

	return bytes;
}

template<typename T> T& Serializer::from_bytes(byte *bytes, T& object)
{
	static_assert(std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

	byte *begin_object = reinterpret_cast<byte*>(std::addressof(object));
	std::vector<byte> bytes_vector(&bytes);
	std::copy(bytes_vector.begin(), bytes_vector.end(), begin_object);

	return object;
}