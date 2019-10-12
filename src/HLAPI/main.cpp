#include "Init.hpp"
#include "Server.hpp"
#include "BitConverter.hpp"
#include "TcpClient.hpp"
#include "InternalTags.hpp"
#include "NetworkMessage.hpp"

#include <iostream>

int main()
{
	std::net::Initialize();
	std::net::Server server(100);
	server.Start();
	
	std::net::TcpClient client;
	std::net::IPAddress ip("127.0.0.1");
	client.Connect(ip);

	std::net::TcpClient client2;
	client2.Connect(ip);

	while (true)
	{
		uint32_t data_size;
		while (client.HasPendingData(data_size))
		{
			std::net::NetworkMessage message;

			uint8_t *bytes = new uint8_t[data_size]();

			int32_t read;
			client.Recv(bytes, data_size, read);

			message.Deserialize(bytes, data_size);

			uint32_t id = std::BitConverter::FromBytes<uint32_t>((uint8_t*)(message.GetData<void>()));
			if (message.GetTag() == (uint32_t)InternalTags::AssignID)
			std::cout << id << std::endl;
		}

		while (client2.HasPendingData(data_size))
		{
			std::net::NetworkMessage message2;

			uint8_t* bytes2 = new uint8_t[data_size]();

			int32_t read2;
			client2.Recv(bytes2, data_size, read2);

			message2.Deserialize(bytes2, data_size);

			uint32_t id2 = std::BitConverter::FromBytes<uint32_t>((uint8_t*)(message2.GetData<void>()));
			if (message2.GetTag() == (uint32_t)InternalTags::AssignID)
				std::cout << id2 << std::endl;
		}
	}
}