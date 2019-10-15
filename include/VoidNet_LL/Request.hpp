// https://github.com/mfichman/http

#pragma once

#include "VoidNet_LL/Uri.hpp"
#include "VoidNet_LL/Headers.hpp"
#include "VoidNet_LL/Enums.hpp"

namespace std::net
{
	class Request 
	{
	public:
		Method GetMethod() const 
		{ 
			return m_method;
		}

		const Uri& GetUri() const 
		{ 
			return m_uri;
		}

		const string& GetPath() const 
		{ 
			return m_uri.GetPath();
		}

		const string& GetData() const 
		{ 
			return m_data;
		}

		const string GetHeaderElement(const string& name) const;

		const Headers& GetHeaders() const 
		{ 
			return m_headers; 
		}

		void SetMethod(Method method);
		void SetUri(const Uri& path);
		void SetData(const string& data);
		void AddHeader(const string& name, const string& value);

	private:
		Method m_method = Method::GET;
		Uri m_uri;
		string m_data;
		Headers m_headers;
	};
}