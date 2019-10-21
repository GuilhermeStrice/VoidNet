#pragma once

namespace std::net
{
	enum class SocketParam
	{
		CanRead,
		CanWrite,
		HasError,
	};

	enum class SocketReturn
	{
		Yes,
		No,
		EncounteredError,
	};

	enum class SocketErrors
	{
		SE_SOCKET_ERROR = -1,
		SE_NO_ERROR,
		SE_EINTR,
		SE_EBADF,
		SE_EACCES,
		SE_EFAULT,
		SE_EINVAL,
		SE_EMFILE,
		SE_EWOULDBLOCK,
		SE_EINPROGRESS,
		SE_EALREADY,
		SE_ENOTSOCK,
		SE_EDESTADDRREQ,
		SE_EMSGSIZE,
		SE_EPROTOTYPE,
		SE_ENOPROTOOPT,
		SE_EPROTONOSUPPORT,
		SE_ESOCKTNOSUPPORT,
		SE_EOPNOTSUPP,
		SE_EPFNOSUPPORT,
		SE_EAFNOSUPPORT,
		SE_EADDRINUSE,
		SE_EADDRNOTAVAIL,
		SE_ENETDOWN,
		SE_ENETUNREACH,
		SE_ENETRESET,
		SE_ECONNABORTED,
		SE_ECONNRESET,
		SE_ENOBUFS,
		SE_EISCONN,
		SE_ENOTCONN,
		SE_ESHUTDOWN,
		SE_ETOOMANYREFS,
		SE_ETIMEDOUT,
		SE_ECONNREFUSED,
		SE_ELOOP,
		SE_ENAMETOOLONG,
		SE_EHOSTDOWN,
		SE_EHOSTUNREACH,
		SE_ENOTEMPTY,
		SE_EPROCLIM,
		SE_EUSERS,
		SE_EDQUOT,
		SE_ESTALE,
		SE_EREMOTE,
		SE_EDISCON,
		SE_SYSNOTREADY,
		SE_VERNOTSUPPORTED,
		SE_NOTINITIALISED,
		SE_HOST_NOT_FOUND,
		SE_TRY_AGAIN,
		SE_NO_RECOVERY,
		SE_NO_DATA,
		SE_UDP_ERR_PORT_UNREACH,
		SE_ADDRFAMILY,
		SE_SYSTEM,
		SE_NODEV,
		SE_GET_LAST_ERROR_CODE,
	};

	enum class SocketType
	{
		Unknown = -1,
		Raw = 3, // SOCK_RAW
		Datagram = 2, //SOCK_DGRAM
		Streaming = 1, //SOCK_STREAM
	};

	enum class AddressFamily
	{
		IPv4 = 2, // AF_INET
		IPv6 = 23 // AF_INET6
	};

	enum class SocketProtocol
	{
		ICMP = 1,
		ICMPIpv6 = 58,
		Tcp = 6,
		Udp = 17,
	};

	enum class SocketReceiveFlags
	{
		None = 0,
		Peek = 2,
		WaitAll = 0x100,
	};

	enum class SocketWaitConditions
	{
		WaitForRead,
		WaitForWrite,
		WaitForReadOrWrite,
	};

	enum class SocketConnectionState
	{
		NotConnected,
		Connected,
		ConnectionError,
	};

	enum class HttpStatus
	{
		INVALID_CODE = 0,
		CONTINUE = 100,
		SWITCHING_PROTOCOLS = 101,
		OK = 200,
		CREATED = 201,
		ACCEPTED = 202,
		NON_AUTHORITATIVE_INFO = 203,
		NO_CONTENT = 204,
		RESET_CONTENT = 205,
		PARTIAL_CONTENT = 206,
		MULTIPLE_CHOICES = 300,
		MOVED_PERMANENTLY = 301,
		FOUND = 302,
		SEE_OTHER = 303,
		NOT_MODIFIED = 304,
		USE_PROXY = 305,
		TEMPORARY_REDIRECT = 307,
		BAD_REQUEST = 400,
		UNAUTHORIZED = 401,
		PAYMENT_REQUIRED = 402,
		FORBIDDEN = 403,
		NOT_FOUND = 404,
		METHOD_NOT_ALLOWED = 405,
		NOT_ACCEPTABLE = 406,
		PROXY_AUTHENTICATION_REQUIRED = 407,
		REQUEST_TIMEOUT = 408,
		CONFLICT = 409,
		GONE = 410,
		LENGTH_REQUIRED = 411,
		PRECONDITION_FAILED = 412,
		REQUEST_ENTITY_TOO_LARGE = 413,
		UNSUPPORTED_MEDIA_TYPE = 415,
		REQUESTED_RANGE_NOT_SATISFIABLE = 416,
		EXPECTATION_FAILED = 417,
		INTERNAL_SERVER_ERROR = 500,
		NOT_IMPLEMENTED = 501,
		BAD_GATEWAY = 502,
		SERVICE_UNAVAILABLE = 503,
		GATEWAY_TIMEOUT = 504,
		VERSION_NOT_SUPPORTED = 505,
	};

	enum class Method 
	{ 
		GET,
		HEAD,
		POST,
		PUT,
		DELETE,
		TRACE,
		CONNECT,
	};
}