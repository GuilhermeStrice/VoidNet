#pragma once

#include <HLAPI/NetworkMessage.hpp>

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