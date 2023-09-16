#include <iostream>
#include "ordered_array.hpp"

int
main() {
	ordered_array arr(10); // Capacity 10, Size 0

	std::cout << arr.size() << std::endl; // Should print 0

	arr.insert(3);
	arr.insert(2);
	arr.insert(1);

	std::cout << arr.size() << std::endl; // Should print 3

	// This should print 1, 2, 3
	for(int i = 0; i < arr.size(); ++i)
		std::cout << arr.at(i) << ", ";
	return 0;
}	
