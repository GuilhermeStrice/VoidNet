#include "VoidNetClient.hpp"
#include "Utility.hpp"
#include "Tags.hpp"

#undef SendMessage

bool VoidNetClientAPI::Connect(const std::string &ip, uint16 port)
{
	client->SetIP(ip);
	client->SetPort(port);
	return client->Connect() == VOID_SUCCESS;
}

bool VoidNetClientAPI::SendMessageToServer(byte tag, byte subject, void *data)
{
	return SendMessage(Server, 0, tag, subject, data);
}

bool VoidNetClientAPI::SendMessageToID(uint16 destination_id, byte tag, byte subject, void *data)
{
	return SendMessage(ID, destination_id, tag, subject, data);
}

bool VoidNetClientAPI::SendMessageToOthers(byte tag, byte subject, void *data)
{
	return SendMessage(Others, 0, tag, subject, data);
}

bool VoidNetClientAPI::SendMessageToAll(byte tag, byte subject, void *data)
{
	return SendMessage(All, 0, tag, subject, data);
}

bool VoidNetClientAPI::SendMessageToAllAndMe(byte tag, byte subject, void *data)
{
	return SendMessage(AllAndMe, 0, tag, subject, data);
}

bool VoidNetClientAPI::SendMessage(byte distribution_mode, uint16 destination_id, byte tag, byte subject, void *data)
{
	NetworkMessage message;
	message.tag = tag;
	message.subject = subject;
	message.data = data;
	message.distribution_mode = distribution_mode;
	message.sender = id;
	message.destination_id = destination_id;
	return client->SendMessage(message) == VOID_SUCCESS;
}

void VoidNetClientAPI::Receive()
{
	receive_thread = std::thread(ProcessAllData);
	receive_thread.detach();
}

void VoidNetClientAPI::ProcessAllData()
{
	while (receive)
	{
		NetworkMessage message = client->ReceiveDataArray();
		if (message.valid)
		{
			if (message.tag == CONNECT)
				OnConnect(message.sender);
			else if (message.tag == DISCONNECT)
				OnDisconnect(message.sender);
			else
				OnMessage(message.sender, message.tag, message.subject, message.data);
		}
	}
}

void VoidNetClientAPI::Disconnect()
{
	Utility::Delete(client);
	receive = false;
	receive_thread.join();
}
