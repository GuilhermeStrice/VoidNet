// https://github.com/mfichman/http

#pragma once

#include <string>

namespace std::net
{
	class Authority 
	{
	public:
		Authority(const string& user, const string& host, uint16_t port);
		Authority();

		const string& GetUser() const 
		{ 
			return m_user;
		}

		const string& GetHost() const 
		{ 
			return m_host;
		}

		uint16_t GetPort() const 
		{ 
			return m_port;
		}

		void SetUser(const string& user);
		void SetHost(const string& host);
		void SetPort(uint16_t port);
	private:
		string m_user;
		string m_host;
		uint16_t m_port;
	};

	class Uri {
	public:
		Uri(const char* value);
		Uri(const string& value);
		Uri();

		const string& GetScheme() const 
		{ 
			return m_scheme;
		}

		const Authority& GetAuthority() const 
		{ 
			return m_authority;
		}

		const string& GetPath() const 
		{ 
			return m_path;
		}

		const string& GetHost() const 
		{
			return m_authority.GetHost();
		}

		uint16_t GetPort() const 
		{ 
			return m_authority.GetPort();
		}

		void SetScheme(const string& scheme);
		void SetAuthority(const Authority& authority);
		void SetPath(const string& path);
	private:
		string m_scheme;
		Authority m_authority;
		string m_path;
	};
}