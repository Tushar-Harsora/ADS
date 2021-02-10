#include <iostream>
#include "Scapegoat.h"

int main() {
	scapegoat_tree<int, int> tree;
	for (int i = 1; i <= 10; i++) {
		tree.insert(i, i * 10);
	}
	std::cout << tree;
	for (int i = 1; i < 15; i++) {
		tree.erase(i);
		std::cout << "After Deleting : " << i << "\n" << tree << std::endl;
	}

	return 0;
}