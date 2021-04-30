#include <iostream>
#include "HashTable.h"
#include <unordered_set>

using namespace std;

struct Hasher {
	uint64_t operator()(int data) {
		uint64_t val = data;
		val = (val ^ (val >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
		val = (val ^ (val >> 27)) * UINT64_C(0x94d049bb133111eb);
		val = val ^ (val >> 31);
		return val;
	}
};
int main() {
	HashTable<int, Hasher> s;
	for (int i = 0; i < 10; i++)
		s.insert(i);
	cout << "After inserting [0-10) = " << s << endl;
	for (int i = 5; i < 10; i++)
		s.erase(i);
	s.erase(1337);
	cout << "After Removing [5,10) and 1337 = " << s << endl;;
	return 0;
}