#pragma once

#ifdef _MSC_VER

	#include <winsock2.h>
	#include <ws2tcpip.h>

	namespace std::net
	{
		namespace priv
		{
			static WSADATA WsaData;
			static bool Initialized;
		}

		inline static bool Initialize()
		{
			if (priv::Initialized) return true;
			return (priv::Initialized = WSAStartup(MAKEWORD(2, 2), &priv::WsaData)) == 0;
		}

		inline static void Cleanup()
		{
			if (priv::Initialized)
			{
				WSACleanup();
				priv::Initialized = false;
			}
		}
	}

#else

	namespace std::net
	{
		inline static bool Initialize() { return true; }
		inline static void Cleanup() { }
	}

#endif