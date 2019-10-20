// https://github.com/mfichman/http

#pragma once

#include <map>
#include <string>

namespace std::net
{
	class Headers 
	{
	public:
		const string operator[](const string &name) const;

		multimap<string, string>::const_iterator begin() const
		{ 
			return m_header.begin();
		}

		multimap<string, string>::const_iterator end() const
		{ 
			return m_header.end();
		}

		void AddHeader(string const& name, string const& value);

		static string const HOST;
		static string const CONTENT_LENGTH;
		static string const ACCEPT_ENCODING;
		static string const CONNECTION;
	private:
		multimap<string, string> m_header;
};
}