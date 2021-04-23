#pragma once

#include <vector>
#include <cstdint>

template<class T, typename Combine>
struct SegmentTree {
private:
	using index = int64_t;
	std::vector<T> m_arr{};
	Combine m_comb{};
	size_t m_size;
	const T dummy;
public:
	SegmentTree(const std::vector<T>& vec, const T& dummy) : dummy(dummy) {
		auto n = vec.size();
		m_size = vec.size();
		m_arr.resize(m_size * 2);

		for (size_t i = 0; i < n; i++) {
			m_arr[n + i] = vec[i];
		}

		for (size_t i = n - 1; i > 0; i--) {		// One Based Indexing
			m_arr[i] = m_comb(m_arr[i * 2], m_arr[i * 2 + 1]);
		}
	}

	T query(index l, index r) {
		T ans = dummy;
		for (l += m_size, r += m_size; l <= r; l = (l + 1) / 2, r = (r - 1) / 2) {
			if (l & 1) {
				ans = m_comb(ans, m_arr[l]);
			}
			if (!(r & 1)) {
				ans = m_comb(ans, m_arr[r]);
			}
		}
		return ans;
	}
};