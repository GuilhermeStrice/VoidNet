#pragma once

#include "VoidNet_LL/Socket.hpp"

//#include <openssl/rand.h>
//#include <openssl/ssl.h>
//#include <openssl/err.h>

namespace std::net
{
	class SecureSocket
	{
	public:
		SecureSocket();

		bool Connect(const IPAddress& addr);
		bool Close() const;
		bool HasPendingData(uint32_t& pendingDataSize) const;
		bool Send(byte* data, int32_t count, int32_t &sent, int flags = 0); // Execute 1 write() syscall
		bool Recv(byte* data, int32_t count, int32_t &read, int flags = 0); // Execte 1 read() syscall
		bool Wait(SocketWaitConditions cond, chrono::milliseconds t) const;
		SocketConnectionState GetConnectionState() const;
		void GetAddress(IPAddress& outAddr) const;
		int32_t GetPort() const;

		void UseCertificateFile(string const& path);
		void UsePrivateKeyFile(string const& path);

	private:
		bool SendRaw(byte* buf, size_t len, int flags = 0);
		bool SendFromBio(int flags = 0);
		bool RecvToBio(int flags = 0);
		void HandleReturn(size_t ret);

		unique_ptr<Socket> m_socket;

		//SSL_CTX* m_context;
		//SSL* m_conn;
		//BIO* m_in;
		//BIO* m_out;
		bool m_eof;
	};
}