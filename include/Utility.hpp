#ifndef UTILITY_HPP
#define UTILITY_HPP

#ifdef _MSC_VER
#pragma once
#endif

struct Utility
{
	static void Delete(void *pointer);
	static void DeleteArray(void *pointer);
};

#endif