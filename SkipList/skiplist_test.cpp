#include "skiplist.h"
#include <string>
#include <iostream>

int main() {
	Skiplist<int, std::string> sl;

	sl.insert(1, "Tushar");
	sl.insert(2, "Vishwa");
	sl.insert(3, "Sakshi");
	sl.insert(4, "Rahul");

	std::cout << "After Insert Operation\n";
	for (int i = 1; i <= 4; i++) {
		auto res = sl.find(i);
		if (res)
			std::cout  << res->m_key << " : " << res->m_value << std::endl;
	}

	for (int i = 1; i <= 2; i++)
		sl.erase(i);

	std::cout << "\nAfter Erasing Key 1 and 2\n";
	for (int i = 0; i <= 4; i++) {
		auto res = sl.find(i);
		if (res)
			std::cout << res->m_key << " : " << res->m_value << std::endl;
	}
	return 0;
}