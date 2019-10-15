// https://github.com/mfichman/http

#include "VoidNet_LL/Headers.hpp"

namespace std::net
{
	string const Headers::HOST("Host");
	string const Headers::CONTENT_LENGTH("Content-Length");
	string const Headers::ACCEPT_ENCODING("Accept-Encoding");
	string const Headers::CONNECTION("Connection");

	const string Headers::operator[](const string & name) const
	{
		auto i = m_header.find(name);
		return (i == m_header.end()) ? "" : i->second;
	}

	void Headers::AddHeader(string const& name, string const& value) 
	{
		m_header.emplace(name, value);
	}
}