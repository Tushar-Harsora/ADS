#pragma once

#include <vector>

template<typename T>
struct Node {
	T value;
	Node* next;
	Node(const T& value) : value(value) {
		next = nullptr;
	}
};

template<typename T, class Hasher>
struct HashTable {
private:
	std::vector<Node<T>*> buckets;
	uint16_t num_elems;
	constexpr static long double max_load_factor = 2.0;
	Hasher m_hash{};
public:
	HashTable() : num_elems(0) {
		buckets.resize(5);
		for (auto& bucket : buckets)
			bucket = nullptr;
	}

	void insert(const T& key) {
		uint64_t hash = m_hash(key);

		uint64_t position = hash % buckets.size();

		if (buckets[position] == nullptr) {
			buckets[position] = new Node(key);
		}
		else {
			// go to last node whose next pointer is nullptr
			Node<T>* last = buckets[position];
			while (last->next)
				last = last->next;
			last->next = new Node(key);
		}
		num_elems++;

		if (load_factor() >= max_load_factor) {
			buckets.resize(2 * buckets.size());
		}
 	}

	long double load_factor() const {
		return static_cast<long double>(num_elems) / buckets.size();
	}
};