#include <iostream>
#include "Queue"

int main() {
	Queue<char> q;

	for (char ch = 'A'; ch <= 'Z'; ch++)
		q.enqueue(ch);

	while (!q.empty()) {
		std::cout << q.front();
		q.dequeue();
	}
	return 0;
}