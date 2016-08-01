#ifndef DEFS_HPP
#define DEFS_HPP

#ifdef _MSC_VER
#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>

typedef signed char int8, sbyte;
typedef unsigned char uint8, byte;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed __int64 int64;
typedef unsigned __int64 uint64;

#ifdef _WIN64
typedef signed __int64 int_ptr;
typedef unsigned __int64 uint_ptr;
#else
typedef signed long int_ptr;
typedef unsigned long uint_ptr;
#endif // win64

#elif defined(__GNUC__) || defined(__clang__)
typedef signed char int8, sbyte;
typedef unsigned char uint8, byte;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

#ifdef _WIN64
typedef signed long long int_ptr, int64;
typedef unsigned long long uint_ptr, uint64;
#else
typedef signed long int_ptr;
typedef unsigned long uint_ptr;

#ifdef __LP64__
typedef signed long int64;
typedef unsigned long uint64;
#else
typedef signed long long int64;
typedef unsigned long long uint64;
#endif // __LP64__

#endif // win64

#elif defined(__DECCXX)

typedef signed char int8, sbyte;
typedef unsigned char uint8, byte;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed __int64 int64;
typedef unsigned __int64 uint64;

#ifdef __VMS
#ifdef __32BITS
typedef signed long int_ptr;
typedef unsigned long uint_ptr;
#else
typedef signed __int64 int_ptr;
typedef unsigned __int64 uint_ptr;
#endif // __32BITS
#else
typedef signed long int_ptr;
typedef unsigned long uint_ptr
#endif // __VMS

#elif defined(__HP_aCC)

typedef signed char int8, sbyte;
typedef unsigned char uint8, byte;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long int_ptr;
typedef unsigned long uint_ptr;

#ifdef __LP64__
typedef signed long int64;
typedef unsigned long uint64;
#else
typedef signed long long int64;
typedef unsigned long long uint64;
#endif // __LP64__

#elif defined(__SUNPRO_CC)

typedef signed char int8, sbyte;
typedef unsigned char uint8, byte;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long int_ptr;
typedef unsigned long uint_otr

#ifdef __sparcv9
typedef signed long int64;
typedef unsigned long uint64;
#else 
typedef signed long long int64;
typedef unsigned long long uint64;
#endif // __sparcv9

#elif defined(__IBMCPP__)

typedef signed char int8, sbyte;
typedef unsigned char uint8, byte;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long int_ptr;
typedef unsigned long uint_ptr;

#ifdef __64BIT__
typedef signed long int64;
typedef unsigned long uint64;
#else
typedef signed long long int64;
typedef unsigned long long uint64;
#endif // __64BIT__

#elif defined(__sgi)

typedef signed char int8, sbyte;
typedef unsigned char uint8, byte;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long int_ptr;
typedef unsigned long uint_ptr;

#if _MIPS_SZLONG == 64
typedef signed long int64;
typedef unsigned long uint64;
#else
typedef signed long long int64;
typedef unsigned long long uint64;
#endif // _MIPS_SZLONG

#elif defined(_DIAB_TOOL)

typedef signed char int8, sbyte;
typedef unsigned char uint8, byte;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int in32;
typedef unsigned int uint32;
typedef signed long int_ptr;
typedef unsigned long uint_ptr;
typedef signed long long int64;
typedef unsigned long long uint64;

#endif // compiler data type defenitions

const uint16 default_client_port = 60250;
const uint16 default_server_port = 61250;

enum DistributionType
{
	All = 1, // Others and Server
	AllAndMe, // Other, Server and Ourselfs
	Server,
	Others,
	ID,
};

enum ConnectionCode
{
	Accept = 0,
	Reject
};

enum InternalTags
{
	CONNECT = 65534,
	DISCONNECT = 65535,
};

#endif // DEFS_HPP