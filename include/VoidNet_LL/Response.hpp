// https://github.com/mfichman/http

#pragma once

#include "VoidNet_LL/Headers.hpp"
#include "VoidNet_LL/Cookies.hpp"
#include "VoidNet_LL/Enums.hpp"

#include <string>

namespace std::net
{
	class Response 
	{
	public:
		Response(const string& text);
		Response() {};

		HttpStatus GetStatus() const 
		{ 
			return m_status; 
		}

		const string& GetData() const 
		{ 
			return m_data; 
		}

		const string GetHeader(const string& name) const;
		const Cookie GetCookie(const string& name) const;

		void SetStatus(HttpStatus status);
		void SetData(const string& data);
		void SetHeader(const string& name, const string& value);
		void SetCookie(const Cookie& cookie);

	private:
		HttpStatus m_status = HttpStatus::INVALID_CODE;
		string m_data;
		Headers m_headers;
		Cookies m_cookies;
	};
}