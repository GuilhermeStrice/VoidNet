#pragma once

#include <VoidNet_HL/NetworkMessage.hpp>

namespace std::net
{
	class Plugin
	{
	public:
		Plugin()
		{
		}

		virtual void HandleMessage(const NetworkMessage& msg) = 0;
	};
}