#include "UdpServer.hpp"
#include "Utility.hpp"

uint16 UdpServer::allocate_id()
{
	for (uint16 i = 1; i < max_connections; ++i)
	{
		bool flag = true;
		for (std::vector<UdpClient>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			UdpClient client = *it;
			if (client.GetID() == i)
			{
				flag = false;
				break;
			}
		}

		if (flag)
			return i;
	}
	return 0;
}

bool UdpServer::initialize(uint16 port)
{
	server_udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (server_udp_socket == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	memset(&server, '\0', sizeof(struct sockaddr_in));

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	server.sin_addr.S_un.S_un_b.s_b1 = 0;
	server.sin_addr.S_un.S_un_b.s_b2 = 0;
	server.sin_addr.S_un.S_un_b.s_b3 = 0;
	server.sin_addr.S_un.S_un_b.s_b4 = 0;

	if (bind(server_udp_socket, reinterpret_cast<struct sockaddr*>(&server), sizeof(struct sockaddr_in)) == -1)
	{
		closesocket(server_udp_socket);
		WSACleanup();
		return false;
	}
	return initialized = true;
}

void UdpServer::shutdown_internal()
{
	WSACleanup();
}