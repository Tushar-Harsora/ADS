#include <iostream>
#include "avl.h"

int main() {
	AVL<int, int> avl1, avl2;

	for (int i = 1; i <= 5; i++) {
		avl1.insert(i, i * 10);
	}

	for (int i = 11; i <= 15; i++) {
		AVL<int, int> temp;
		temp.insert(i, i * 10);
		// cout << avl1 << endl << endl;
		avl1.root = join(avl1, temp);
		// cout << avl1 << endl << endl;
	}

	//auto ret = join(avl1, avl2);
	cout << avl1;
	/*for (int i = 0; i < 15; i++) {
		avl.erase(i);
		cout << "After Removing : " << i << " :: " << avl << endl;
	}*/
	return 0;
}