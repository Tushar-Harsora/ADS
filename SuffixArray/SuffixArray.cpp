#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

vector<pair<string, int>> build_suffix_array(const string& s) {
	auto n = s.size();
	vector<pair<string, int>> suffixes;
	for (int i = 0; i < n; i++) {
		suffixes.emplace_back(s.substr(i, n - i), i);
	}
	sort(suffixes.begin(), suffixes.end());
	return suffixes;
}

auto get_occurences(const vector<pair<string, int>>& suffix_arr, const string& needle) {
	int n = suffix_arr.size(), m = needle.size();
	int low = 0, high = n - 1;
	int first_of = INT_MAX, last_of = INT_MIN;
	while (low <= high) {
		int mid = low + (high - low) / 2;

		const string& curr = suffix_arr[mid].first;
		int mx = min((int)curr.size(), m);
		bool done = false;
		for (int i = 0; i < mx; i++) {
			int res = curr[i] - needle[i];
			if (res > 0) {
				high = mid - 1;
				done = true;
				break;
			}
			if (res < 0) {
				low = mid + 1;
				done = true;
				break;
			}
		}
		if (!done) {
			if (curr.size() < m) {
				low = mid + 1;
			}
			else if (curr.size() > m) {
				first_of = mid;
				high = mid - 1;
			}
			else {
				first_of = mid;
				high = mid - 1;
			}
		}
	}

	low = first_of, high = n - 1;
	while (low <= high) {
		int mid = low + (high - low) / 2;

		const string& curr = suffix_arr[mid].first;
		int mx = min((int)curr.size(), m);
		bool done = false;
		for (int i = 0; i < mx; i++) {
			int res = curr[i] - needle[i];
			if (res > 0) {
				high = mid - 1;
				done = true;
				break;
			}
			if (res < 0) {
				low = mid + 1;
				done = true;
				break;
			}
		}
		if (!done) {
			if (curr.size() < m) {
				low = mid + 1;
			}
			else if (curr.size() > m) {
				last_of = mid;
				low = mid + 1;
			}
			else {
				last_of = mid;
				low = mid + 1;
			}
		}
	}

	return last_of - first_of + 1;
}

int main() {
	string s = "banana";
	auto suffix_array = build_suffix_array(s);

	cout << "The \"an\" occurs " << get_occurences(suffix_array, "an") << " times in banana" << endl;
	cout << "The \"na\" occurs " << get_occurences(suffix_array, "na") << " times in banana" << endl;
	cout << "The \"ban\" occurs " << get_occurences(suffix_array, "ban") << " times in banana" << endl;
	return 0;
}