#pragma once

#include <VoidNet_HL/NetworkMessage.hpp>
#include <VoidNet_HL/InternalTags.hpp>
#include <string>

namespace std::net
{
	class Plugin
	{
	public:
		Plugin()
		{
		}

		void HandleMessage(const NetworkMessage& msg)
		{
			if (msg.GetTag() == (uint32_t)InternalTags::Disconnect)
			{
				OnDisconnect(*(msg.GetData<string>()));
			}
			else if (msg.GetTag() == (uint32_t)InternalTags::Connect)
			{
				OnNewConnection(msg.GetSenderID(), msg.GetData<void>());
			}
			else if (msg.GetTag() == (uint32_t)InternalTags::AssignID)
			{
				OnConnection();
			}
			else
			{
				OnDataReceived(msg.GetSenderID(), msg.GetDistributionMode(), msg.GetDestinationID(), msg.GetTag(), msg.GetData<void>());
			}
		}

		virtual void OnDisconnect(string) abstract;
		virtual void OnNewConnection(uint32_t, void*) abstract;
		virtual void OnConnection() abstract;
		virtual void OnDataReceived(uint32_t, DistributionMode, uint32_t, uint32_t, void*) abstract;
	};
}