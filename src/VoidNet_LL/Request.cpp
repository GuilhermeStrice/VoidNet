// https://github.com/mfichman/http

#include "VoidNet_LL/Request.hpp"

namespace std::net
{
	const string Request::GetHeaderElement(const string& name) const 
	{
		return m_headers[name];
	}

	void Request::SetMethod(Method method) 
	{
		m_method = method;
	}

	void Request::SetUri(const Uri& uri) 
	{
		m_uri = uri;
	}

	void Request::SetData(const string& data) 
	{
		m_data = data;
	}

	void Request::AddHeader(const string& name, const string& value) 
	{
		m_headers.AddHeader(name, value);
	}
}