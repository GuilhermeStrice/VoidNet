// https://github.com/mfichman/http

#pragma once

#include "VoidNet_LL/Response.hpp"
#include "VoidNet_LL/Request.hpp"

namespace std::net
{
	class Http
	{
	public:
		static Response Get(string const& path, string const& data = "");
		static Response Post(string const& path, string const& data = "");

	private:
		static Response Send(Request const& request);
		static string Str(Request const& request);
	};
}