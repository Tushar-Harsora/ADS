#include <iostream>
#include "avl.h"

int main() {
	AVL<int, int> avl;

	for (int i = 1; i <= 10; i++) {
		avl.insert(i, i * 10);
	}

	std::cout << avl << "\n";

	for (int i = 0; i < 15; i++) {
		avl.erase(i);
		cout << "After Removing : " << i << " :: " << avl << endl;
	}
	return 0;
}