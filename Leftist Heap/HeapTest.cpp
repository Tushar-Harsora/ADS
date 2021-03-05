#include <iostream>
#include "LeftistHeap.h"

using namespace std;

int main() {
	leftist_heap<int> hp;
	for (int i = 0; i < 10; i++)
		hp.insert(i + 1);
	
	std::cout << "After Inserting [0-10]\n";
	hp.vertical_print();
	std::cout << "\n";

	cout << "Current Min Element : " << hp.top() << "\n";
	hp.pop();
	cout << "After Poping Min Element\n";
	hp.vertical_print();
	return 0;
}