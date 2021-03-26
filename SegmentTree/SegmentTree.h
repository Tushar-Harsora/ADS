#pragma once

#include <vector>
#include <cstdint>

template<class T, typename Combine, class _Container = std::vector<T>>
struct SegmentTree {
private:
	using index = int64_t;
	_Container m_arr{};
	Combine m_comb{};
	size_t m_size;
	const T dummy;

	constexpr static size_t next_power_of_2(size_t num) {
		if (num == 1)
			return 2;
		return (1LL << size_t(log2(num - 1) + 1LL));
	}
public:
	SegmentTree(const std::vector<int>& vec, const T& dummy) : dummy(dummy) {
		size_t sz = next_power_of_2(vec.size());
		m_arr.resize(2 * sz, dummy);
		m_size = 2 * sz;
		for (size_t i = sz - 1, k = 0LL; k < vec.size() && i < 2 * sz; i++, k++) {
			m_arr[i] = vec[k];
		}
		index i = sz - 1ll;
		do{
			i--;
			m_arr[i] = m_comb(m_arr[2 * i + 1], m_arr[2 * i + 2]);
		} while (i > 0);
	}

	T query(index l, index r) {
		return _query(0, m_size / 2 - 1LL, m_size - 2, l + m_size / 2 - 1LL, r + m_size / 2 - 2LL);
	}

	T _query(index curr, index l, index r, index fl, index fr) {
		if (l > r)
			return dummy;
		if (r < fl || l > fr)
			return dummy;
		if (l >= fl && r <= fr)
			return m_arr[curr];
		m_comb(_query(2 * curr + 1, l, r / 2, fl, fr), _query(2 * curr + 2, r / 2 + 1, r, l, r));
	}
};