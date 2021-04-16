#include "HashTable.h"
#include <unordered_set>

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
	for (int i = 0; i < 12; i++) {
		s.insert(i);
	}
	return 0;
}