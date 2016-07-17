#include "NetworkBuffer.hpp"
#include "Utility.hpp"

NetworkBuffer::NetworkBuffer()
{
}

NetworkBuffer::~NetworkBuffer()
{
	Utility::DeleteArray(body);
}