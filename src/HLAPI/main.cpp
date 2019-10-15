#include "VoidNet/Init.hpp"
#include "HLAPI/Server.hpp"
#include "HLAPI/ByteConverter.hpp"
#include "VoidNet/TcpClient.hpp"
#include "HLAPI/InternalTags.hpp"
#include "HLAPI/NetworkMessage.hpp"
#include "HLAPI/Plugin/Plugin.hpp"

#include <iostream>

class Plugin : public std::net::Plugin
{
	virtual void HandleMessage(const std::net::NetworkMessage& message) override
	{
		
	}
};

int main()
{
	std::net::Initialize();
	std::net::Server server(100);
	server.AddPlugin(new Plugin());
	server.Start();
	
	std::net::TcpClient client;
	std::net::IPAddress ip("127.0.0.1");
	client.Connect(ip);

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

			uint32_t id = std::ByteConverter::FromBytes<uint32_t>((uint8_t*)(message.GetData<void>()));
			if (message.GetTag() == (uint32_t)InternalTags::AssignID)
			{
				std::cout << id << std::endl;

				std::string str = std::string("asd");
				std::net::NetworkMessage msg(100, std::net::DistributionMode::AllAndServer, 150, 1, &str, sizeof(str));

				uint32_t dataa_size;
				uint8_t* dataaaa = msg.SerializeData(dataa_size);
				int32_t sent;
				client.Send(dataaaa, dataa_size, sent);
			}
			else
			{
				std::string* txt = message.GetData<std::string>();
				std::cout << txt->c_str() << std::endl;
			}
		}
	}
}