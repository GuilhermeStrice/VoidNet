#include "Serializer.hpp"

template<typename T> std::array<byte, sizeof(T)> Serializer::to_bytes(const T& object)
{
	std::array<byte, sizeof(T)> bytes;

	const byte *begin = reinterpret_cast<const byte*>(std::addressof(object));
	const byte *end = begin + sizeof(T);
	std::copy(begin, end, std::begin(bytes));

	return bytes;
}

template<typename T> T& Serializer::from_bytes(const std::array<byte, sizeof(T)> &bytes, T& object)
{
	static_assert(std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

	byte *begin_object = reinterpret_cast<byte*>(std::addressof(object));
	std::copy(std::begin(bytes), std::end(bytes), begin_object);

	return object;
}