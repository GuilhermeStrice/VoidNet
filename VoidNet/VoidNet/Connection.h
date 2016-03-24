#pragma once

#include <Windows.h>
#include <WinSock2.h>
#include <string>

namespace VoidNet
{
	struct Connection
	{
		// Default constructor
		Connection();

		// Using this constructor will connect automatically
		// PortNumber grater than 0
		// IPAddress cant be null
		Connection(std::string, unsigned int);

		// Destructor
		~Connection();

		// Assignment operator
		VoidNet::Connection &operator = (const Connection&);
		bool operator == (Connection&);
		bool operator != (Connection&);

		// this will return the IPAddress of the client connected
		std::string getIPAddress();

		// this will return the Port Number of the client connected
		unsigned int getPortNumber();


			
	private:
		// connection socket
		SOCKET *socket;

		// client ip address
		std::string IPAddress;
		
		// client port
		unsigned int PortNumber;
	};
}