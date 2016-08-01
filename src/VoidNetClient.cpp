#include "VoidNetClient.hpp"
#include "Utility.hpp"
#include "Tags.hpp"

#include <future>

#undef SendMessage

bool VoidNetClientAPI::Connect(const std::string &ip, uint16 port)
{
	client.SetIP(ip);
	client.SetPort(port);
	return client.Connect();
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
	NetworkMessage message;
	message.tag = tag;
	message.subject = subject;
	message.data = data;
	message.distribution_mode = distribution_mode;
	message.sender = id;
	message.destination_id = destination_id;
	client.SendMessage(message);
}

void VoidNetClientAPI::Receive()
{
	std::async(std::launch::async, &process_all_data);
}

void VoidNetClientAPI::process_all_data()
{
	client.ReceiveMessages();
}

void VoidNetClientAPI::Disconnect()
{
	receive = false;
}
