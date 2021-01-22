#include <iostream>
#include "avl.h"

int main() {
	AVL<int, int> avl;

	for (int i = 1; i <= 10; i++) {
		avl.insert(i, i * 10);
	}

	std::cout << avl << "\n";
	return 0;
}