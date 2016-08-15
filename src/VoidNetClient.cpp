#include "VoidNetClient.hpp"
#include "Utility.hpp"

#include <future>

bool VoidNetClientAPI::Connect(const std::string &ip, uint16 port)
{
	tcp_client.SetIP(ip);
	tcp_client.SetPort(port);
	return tcp_client.Connect();
}

void VoidNetClientAPI::SendMessageToServer(byte tag, byte subject, void *data)
{
	SendMessage(Server, 0, tag, subject, data);
}

void VoidNetClientAPI::SendMessageToID(uint16 destination_id, byte tag, byte subject, void *data)
{
	SendMessage(ID, destination_id, tag, subject, data);
}

void VoidNetClientAPI::SendMessageToOthers(byte tag, byte subject, void *data)
{
	SendMessage(Others, 0, tag, subject, data);
}

void VoidNetClientAPI::SendMessageToAll(byte tag, byte subject, void *data)
{
	SendMessage(All, 0, tag, subject, data);
}

void VoidNetClientAPI::SendMessageToAllAndMe(byte tag, byte subject, void *data)
{
	SendMessage(AllAndMe, 0, tag, subject, data);
}

void VoidNetClientAPI::SendMessage(byte distribution_mode, uint16 destination_id, byte tag, byte subject, void *data)
{
	if (tag != CONNECT && tag != DISCONNECT)
	{
		NetworkMessage message;
		message.tag = tag;
		message.subject = subject;
		message.data = data;
		message.distribution_mode = distribution_mode;
		message.sender = id;
		message.destination_id = destination_id;
		tcp_client.SendMessage(message);
	}
}

void VoidNetClientAPI::Receive()
{
	std::async(std::launch::async, &receive_data);
}

void VoidNetClientAPI::receive_data()
{
	tcp_client.ReceiveMessages();
}

void VoidNetClientAPI::Disconnect()
{
	tcp_client.Shutdown();
}
