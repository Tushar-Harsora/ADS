#include "SegmentTree.h"
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct min_fun {
	int operator()(const int& lhs, const int& rhs) {
		return std::min(lhs, rhs);
	}
};

int main() {
	cout << log2(1) << endl;
	int n;
	cin >> n;
	vector<int> arr(n);
	for (auto& ele : arr)
		cin >> ele;

	SegmentTree<int, min_fun> s(arr, INT_MAX);

	int q;
	cin >> q;
	while (q--) {
		int x, y;
		cin >> x >> y;
		cout << s.query(x, y) << endl;
	}

	return 0;
}