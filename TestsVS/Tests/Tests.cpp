#include "Util.hpp"

#include <iostream>

int main()
{
	std::vector<byte> bytes = Utility::BitConverter::FromInt32(50000);
	int32 integer = Utility::BitConverter::ToInt32(bytes);
	std::cout << integer << std::endl;
	std::cin.get();
	return 0;
}

