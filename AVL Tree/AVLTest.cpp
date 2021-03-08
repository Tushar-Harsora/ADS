#include <iostream>
#include "avl.h"

int main() {
	auto prepare_data = []() {
		AVL<int, int> avl1;

		for (int i = 1; i <= 5; i++) {
			avl1.insert(i, i * 10);
		}

		for (int i = 11; i <= 15; i++) {
			AVL<int, int> temp;
			temp.insert(i, i * 10);
			avl1.root = AVL<int, int>::join(avl1, temp);
		}

		return avl1;
	};
	{
		// Split At Given Key left tree will be All keys Less Than 10, right tree will be keys Greater Than Equal To 10
		cout << "Splitting On Key 10" << endl;
		auto splitted = AVL<int, int>::split(prepare_data(), 10);
		cout << splitted.first << endl << splitted.second;
	}

	{
		// Split From Root
		auto avl_tree = prepare_data();
		cout << "\n\nSplitting From Root\nThe Key At Root is : " << avl_tree.root->key;
		auto splitted = AVL<int, int>::split(avl_tree, avl_tree.root->key);
		cout << splitted.first << endl << splitted.second;
	}
	return 0;
}