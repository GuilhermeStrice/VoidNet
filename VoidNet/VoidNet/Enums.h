#pragma once

namespace VoidNet
{
	enum Protocol
	{
		VOID_TCP,
		VOID_UDP
	};

	enum ErrorCode
	{
		VOID_SUCCESS,
		VOID_WSA_STARTUP_FAIL,
		VOID_GET_ADDR_INFO_FAIL,
		VOID_COULDT_CREATE_SOCKET,
		VOID_SOCKET_ERROR
	};

	enum SocketType
	{
		VOID_CLIENT,
		VOID_SERVER
	};
}