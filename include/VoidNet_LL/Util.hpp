#pragma once

#include <vector>
#include <sstream>

#include "VoidNet_LL/Net.hpp"

namespace std::net
{
	static vector<string> Split(const string& str, const string& delimiter);
	static sockaddr_in CreateAddress(uint32_t address, uint16_t port);
}