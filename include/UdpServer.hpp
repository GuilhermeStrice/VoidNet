#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "Defs.hpp"
#include "UdpClient.hpp"

#include <string>
#include <vector>

struct UdpServer
{
private:
	bool initialize(uint16 port = default_server_port);

	uint16 allocate_id();

	void shutdown_internal();

	bool initialized = false;
	bool running = false;

	uint16 max_connections = 0;

	std::vector<UdpClient> clients;

	SOCKET server_udp_socket = INVALID_SOCKET;
	struct sockaddr_in server;
};

#endif