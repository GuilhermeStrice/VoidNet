#include "Serializer.hpp"

template<typename T> const std::vector<byte> &Serializer::to_bytes(const T& object)
{
	std::vector<byte> bytes;

	const byte *begin = reinterpret_cast<const byte*>(std::addressof(object));
	const byte *end = begin + sizeof(T);
	std::copy(begin, end, bytes.begin());

	return bytes;
}

template<typename T> const T& Serializer::from_bytes(const std::vector<byte> &bytes, T& object)
{
	static_assert(std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");
	std::copy(bytes.begin(), bytes.end(), reinterpret_cast<const byte*>(std::addressof(object));

	return object;
}