#include "VoidNet_LL/Util.hpp"

namespace std::net
{
	vector<string> Split(const string& str, const string& delimiter)
	{
		vector<string> splited;
		if (str.empty() && delimiter.empty())
			return vector<string>();
		string::size_type lastPos = str.find_first_not_of(delimiter, 0);
		string::size_type pos = str.find_first_of(delimiter, lastPos);

		while (string::npos != pos || string::npos != lastPos)
		{
			splited.push_back(str.substr(lastPos, pos - lastPos));
			lastPos = str.find_first_not_of(delimiter, pos);
			pos = str.find_first_of(delimiter, lastPos);
		}
		return splited;
	}

	sockaddr_in CreateAddress(uint32_t address, uint16_t port)
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = htonl(address);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		return addr;
	}
}