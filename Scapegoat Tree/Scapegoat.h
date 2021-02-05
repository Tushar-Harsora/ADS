#ifndef SCAPEGOAT_H
#define SCAPEGOAT_H

#include <iostream>
#include <cassert>
#include <vector>
#include <tuple>
#include <queue>

template<typename Key, typename Value>
struct Node {
public:
	Node() = default;
	Node(Key key, Value value) : key(key), value(value),
		left(nullptr), right(nullptr) { }

	Key key;
	Value value;
	Node<Key, Value>* left;
	Node<Key, Value>* right;

	size_t size() const {
		int count = 1;
		std::queue<Node<Key, Value>*> q;
		q.push(left);
		q.push(right);

		while (!q.empty()) {
			auto f = q.front();
			q.pop();
			if (f != nullptr) {
				q.push(f->left);
				q.push(f->right);
				count++;
			}
		}

		return count;
	}

	friend std::ostream& operator<<(std::ostream& out, const Node<Key, Value>* const node) {
		if (node == nullptr)
			return out;
		// Or Instead You Can Prematurely Check if left or right child exists
		// if yes then call operator<< recursively
		out << node->left;
		out << node->key << " : " << node->value << "\n";
		out << node->right;

		return out;
	}
};

template<typename Key, typename Value>
struct scapegoat_tree {
public:
	scapegoat_tree() : m_root(nullptr), num_nodes(0) { }

	void insert(const Key& key, const Value& value) {
		auto new_node = new Node<Key, Value>(key, value);
		size_t depth = 0;
		m_root = _insert(m_root, new_node, depth);
	}
private:
	Node<Key, Value>* _insert(Node<Key, Value>* root, Node<Key, Value>* new_node, size_t& depth) {
		if (root == nullptr) {
			num_nodes++;
			return new_node;
		}

		bool inserted_left = true;	// for checking if i am inserting to right or left
									// Useful when unrolling from recursion to check which branch
									// was taken
		if (new_node->key < root->key) {
			root->left = _insert(root->left, new_node, ++depth);
		}
		else if (new_node->key > root->key) {
			root->right = _insert(root->right, new_node, ++depth);
			inserted_left = false;
		}
		else {
			assert(false);
		}

		bool finding_scapegoat = depth > log3by2(num_nodes);
		if (!finding_scapegoat)
			return root;

		size_t my_size = root->size();
		bool i_am_scapegoat = ceill(factor * my_size) < (inserted_left ? root->left->size() : root->right->size());
		if (!i_am_scapegoat)
			return root;

		size_t ind = 0;
		std::vector<Node<Key, Value>*> store(my_size);
		to_array(root, store, ind);

		ind = 0;
		create_balanced_tree(store, ind);
	}

	static size_t log3by2(size_t value) {
		static long double log2offactor = log2l(factor);
		return std::ceill(log2l((double)value) / log2offactor);
	}

	static void to_array(const Node<Key, Value>* r, std::vector<Node<Key, Value>*>& store, size_t& ind) {
		if (r == nullptr)
			return;

		to_array(r->left, store, ind);
		store[ind] = r;
		ind++;
		to_array(r->right, store, ind);

		return;
	}
	Node<Key, Value>* m_root;
	size_t num_nodes;
	constexpr static double factor = 2.0 / 3.0;
};

#endif // !SCAPEGOAT_H