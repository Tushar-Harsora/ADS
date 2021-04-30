#include <iostream>
#include <vector>
#include <array>

using namespace std;

template<int size>
struct dsu {
	array<int, size> par;

	dsu() {
		for (int i = 0; i < size; i++)
			par[i] = i;
	}

	int find(int root) {
		if (root == par[root])
			return root;

		return par[root] = find(par[root]);
	}

	void union_dsu(int x, int y) {
		int xroot = find(x);
		int yroot = find(y);
		if (xroot != yroot) {
			par[xroot] = yroot;
		}
	}

	void list_set(int root) {
		int target = find(root);
		for (int i = 0; i < size; i++) {
			if (find(i) == target) {
				cout << i << "\t";
			}
		}
		cout << endl;
	}

	friend ostream& operator<<(ostream& stream, dsu<size>& d) {
		for (int i = 0; i < size; i++) {
			stream << i << " = " << d.find(i) << "\t";
		}
		return stream;
	}
};

int main() {
	dsu<10> d;
	d.union_dsu(0, 2);
	d.union_dsu(2, 5);
	d.union_dsu(5, 7);
	d.union_dsu(8, 9);

	d.list_set(0);
	cout << d << endl;
}