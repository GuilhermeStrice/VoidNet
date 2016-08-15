#ifndef VOID_NET_SERVER_HPP
#define VOID_NET_SERVER_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "TcpServer.hpp"

struct VoidNetServer
{
private:
	TcpServer server;
};

#endif