// https://github.com/mfichman/http

#pragma once

#include <map>

namespace std::net
{
	class Cookie 
	{
	public:
		Cookie(const string& text);

		Cookie() : m_httpOnly(false), m_secure(false)
		{
		}

		const string& GetName() const 
		{ 
			return m_name;
		}

		const string& GetValue() const 
		{ 
			return m_value;
		}

		const string& GetPath() const 
		{ 
			return m_path;
		}

		bool IsHttpOnly() const 
		{ 
			return m_httpOnly;
		}

		bool IsSecure() const 
		{ 
			return m_secure;
		}

		void SetName(const string& name) 
		{ 
			m_name = name;
		}

		void SetValue(const string& value) 
		{ 
			m_value = value;
		}

		void SetPath(const string& path) 
		{ 
			m_path = path;
		}

		void SetHttpOnly(bool httpOnly) 
		{ 
			m_httpOnly = httpOnly;
		}

		void SetSecure(bool secure) 
		{ 
			m_secure = secure;
		}

	private:
		string m_name;
		string m_value;
		string m_path;
		bool m_httpOnly;
		bool m_secure;
	};

	class Cookies 
	{
	public:
		const Cookie operator[](const string &name) const;

		map<string, Cookie>::const_iterator begin() const
		{ 
			return m_cookie.begin();
		}

		map<string, Cookie>::const_iterator end() const
		{ 
			return m_cookie.end();
		}

		void SetCookie(Cookie const& cookie);

		static const string HOST;
		static const string CONTENT_LENGTH;
		static const string ACCEPT_ENCODING;
		static const string CONNECTION;
	private:
		map<string, Cookie> m_cookie;
	};
}