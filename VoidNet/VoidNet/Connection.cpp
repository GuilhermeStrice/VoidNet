#include "Connection.h"

VoidNet::Connection::Connection()
{
}

VoidNet::Connection::Connection(std::string IPAddress, unsigned int PortNumber) : 
	IPAddress(IPAddress), PortNumber(PortNumber)
{
}

VoidNet::Connection::~Connection()
{
	delete socket;
}

VoidNet::Connection &VoidNet::Connection::operator=(const Connection &con)
{
	return (VoidNet::Connection)con;

	VoidNet::Connection asd;
	VoidNet::Connection dsa;
}

bool VoidNet::Connection::operator==(Connection &con)
{
	//return socket == con.socket;
	return false;
}

bool VoidNet::Connection::operator!=(Connection &con)
{
	//return socket != con.socket;
	return false;
}

std::string VoidNet::Connection::getIPAddress()
{
	return IPAddress;
}

unsigned int VoidNet::Connection::getPortNumber()
{
	return PortNumber;
}
