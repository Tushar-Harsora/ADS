#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <vector>
#include <cassert>
#include <random>
#include <iostream>

template<typename Key, typename Value>
struct skip_node {
	Key m_key;
	Value m_value;
	size_t m_level;

	std::vector<skip_node*> forward;

	skip_node(Key key, Value value, size_t level) : m_key(key), m_value(value), m_level(level) {
		forward.resize(level + 1);
		for (size_t i = 0; i <= level; i++)
			forward[i] = nullptr;
	}
};

template<typename Key, 
	typename Value, double Probability = 0.5, size_t max_level = 3,
	typename = typename std::enable_if<std::is_arithmetic<Key>::value, Key>::type>
struct Skiplist {
private:
	skip_node<Key, Value>* head;
	skip_node<Key, Value>* NIL;
	mutable std::random_device rd;
	mutable std::uniform_real_distribution<double> distrib;

	size_t get_random_level() const {
		size_t lev = 1;
		while (distrib(rd) < Probability && lev < max_level)
			lev++;
		return lev;
	}

	static size_t node_level(const std::vector<skip_node<Key, Value>*>& arr) {
		int curr = 1;
		Key nil_key = std::numeric_limits<Key>::max();

		for (size_t i = 0; i < arr.size(); i++) {
			if (arr[i] && arr[i]->m_key != nil_key) {
				curr++;
			}
			else {
				break;
			}
		}
		return curr;
	}
public:
	Skiplist() {
		distrib = std::uniform_real_distribution(0.0, 1.0);
		Key min_value = std::numeric_limits<Key>::min();
		//Value v;		// Default Constructed Value. we will not use it anyways
		head = new skip_node<Key, Value>(min_value, Value(), max_level);
		Key max_value = std::numeric_limits<Key>::max();
		NIL = new skip_node<Key, Value>(max_value, Value(), max_level);

		for (size_t i = 0; i < head->forward.size(); i++)
			head->forward[i] = NIL;
	}

	const skip_node<Key, Value>* find(const Key& key) const {
		skip_node<Key, Value>* ret = head;
		size_t currMax = node_level(head->forward);

		for (size_t i = currMax ; i-- > 0;) {
			while (ret->forward[i] && ret->forward[i]->m_key < key) {
				ret = ret->forward[i];
			}
		}

		ret = ret->forward[0];
		if (ret->m_key == key)
			return ret;
		return nullptr;
	}

	void insert(const Key& key, const Value& value) {
		std::vector<skip_node<Key, Value>*> to_update(head->forward);
		size_t currMax = node_level(head->forward);
		skip_node<Key, Value>* node = head;

		for (size_t i = currMax; i-- > 0;) {
			while (node->forward[i] && node->forward[i]->m_key < key) {
				node = node->forward[i];
			}
			to_update[i] = node;
		}

		node = node->forward[0];

		if (node->m_key != key) {
			size_t new_node_level = get_random_level();
			size_t curr_level = node_level(to_update);

			if (new_node_level > curr_level) {
				for (size_t i = curr_level + 1; i < new_node_level; i++) {
					to_update[i] = node;
				}
			}

			node = new skip_node<Key, Value>(key, value, new_node_level);

			for (size_t i = 0; i < new_node_level; i++) {
				node->forward[i] = to_update[i]->forward[i];
				to_update[i]->forward[i] = node;
			}
		}
		else {
			assert(false);
			node->m_value = value;
			return;
		}
	}

	void erase(Key key) {
		skip_node<Key, Value>* node = head;
		std::vector<skip_node<Key, Value>*> to_update(head->forward);
		size_t currMax = node_level(to_update);

		for (size_t i = currMax; i-- > 0;) {
			while (node->forward[i] && node->forward[i]->m_key < key) {
				node = node->forward[i];
			}
			to_update[i] = node;
		}

		node = node->forward[0];

		if (node->m_key == key) {	// Found Key
			for (size_t i = 0; i < to_update.size(); i++) {
				if (to_update[i]->forward[i] != node)
					break;
				to_update[i]->forward[i] = node->forward[i];
			}

			delete node;
		}
		else {						// Key Not Found Then No Problem

		}
	}

	~Skiplist() {
		delete head;
		delete NIL;
	}
};

#endif // !SKIPLIST_H