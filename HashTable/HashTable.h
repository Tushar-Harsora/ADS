#pragma once

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

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

	void resize_and_rehash() {
		std::vector<Node<T>*> new_buckets;
		new_buckets.resize(2 * buckets.size());

		// insert every items from buckets to new_buckets
		for (size_t i = 0; i < buckets.size(); i++) {
			auto ptr = buckets[i];
			while (ptr) {
				const T& key = ptr->value;
				auto hash = m_hash(key) % new_buckets.size();
				Node<T>* curr = ptr;
				ptr = ptr->next;
				curr->next = nullptr;

				if (new_buckets[hash]) {
					auto first = new_buckets[hash];
					while (first->next)
						first = first->next;
					first->next = curr;		// No need to construct new node
				}
				else {
					new_buckets[hash] = curr;
				}
			}
		}

		buckets = std::move(new_buckets);
		return;
	}
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
			resize_and_rehash();
		}
	}

	void erase(const T& key) {
		uint64_t hash = m_hash(key) % buckets.size();
		Node<T>* prev = buckets[hash];

		// if we find element with specified with key then delete else do nothing
		if (prev) {
			auto target = buckets[hash]->next;
			if (prev->value == key) {
				buckets[hash] = prev->next;
				delete prev;
				return;
			}
			if (target && target->value == key) {
				prev->next = target->next;
				delete target;
				return;
			}
			while (target && target->value != key) {
				prev = target;
				target = target->next;
			}
			if (target == nullptr) {		// we didn't find key
				return;
			}
			assert(target->value == key);
			Node<T>* temp = target;
			prev->next = target->next;
			delete temp;
		}
	}

	long double load_factor() const {
		return static_cast<long double>(num_elems) / buckets.size();
	}

	friend ostream& operator<<(ostream& out, HashTable<T, Hasher>& tab) {
		for (Node<T>* bucket : tab.buckets) {
			if (!bucket)
				continue;
			while (bucket) {
				out << bucket->value << " ";
				bucket = bucket->next;
			}
		}
		return out;
	}
};