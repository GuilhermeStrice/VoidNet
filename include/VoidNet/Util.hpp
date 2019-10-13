#pragma once

#include <vector>
#include <sstream>

#include "Net.hpp"

namespace std::net
{
	static std::vector<std::string> Split(const std::string& str, const std::string& delimiter);
	static sockaddr_in CreateAddress(uint32_t address, uint16_t port);
}