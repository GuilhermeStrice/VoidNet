#include "Utility.hpp"

#include <iostream>

int main()
{
	byte *bytes = Utility::BitConverter::FromUint8(20);
	std::cout << Utility::BitConverter::ToUint8(bytes, 0) << std::endl;
	getchar();
	return 0;
}

