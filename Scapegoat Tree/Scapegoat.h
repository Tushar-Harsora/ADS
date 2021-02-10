#ifndef SCAPEGOAT_H
#define SCAPEGOAT_H

#include <iostream>
#include <cassert>
#include <vector>
#include <tuple>
#include <queue>
#include <cstdint>

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

	friend void swap(Node<Key, Value>& lhs, Node<Key, Value>& rhs) {
		using std::swap;
		swap(lhs.key, rhs.key);
		swap(lhs.value, rhs.value);
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
		size_t depth = 1;
		m_root = _insert(m_root, new_node, depth);
	}

	void erase(const Key& key) {
		m_root = _delete(m_root, key);
	}

	friend std::ostream& operator<<(std::ostream& out, const scapegoat_tree<Key, Value>& tree) {
		out << tree.m_root;
		return out;
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
		bool i_am_scapegoat = my_size < ceill((inserted_left ? root->left->size() : root->right->size())* factor);
		if (!i_am_scapegoat)
			return root;

		size_t ind = 0;
		std::vector<Node<Key, Value>*> store(my_size);
		to_array(root, store, ind);

		ind = 0;
		return create_balanced_tree(store, ind, store.size());
	}

	Node<Key, Value>* _delete(Node<Key, Value>* root, const Key& key) {
		if (root == nullptr) {
			return nullptr;
		}

		bool deleted_left = true;	// for checking if i am inserting to right or left
									// Useful when unrolling from recursion to check which branch
									// was taken
		if (key < root->key) {
			root->left = _delete(root->left, key);
		}
		else if (key > root->key) {
			root->right = _delete(root->right, key);
			deleted_left = false;
		}
		else {
			auto exactly_one_child = (root->left == nullptr || root->right == nullptr) &&
									 !(root->left == nullptr && root->right == nullptr);	// A exor B
			
			if (exactly_one_child) {
				auto child_node = root->left ? root->left : root->right;
				delete root;
				return child_node;
			}
			else if (root->left == nullptr && root->right == nullptr) {
				delete root;
				return nullptr;
			}
			else {
				Node<Key, Value>* inorder_successor = root->right;
				Node<Key, Value>* inorder_successor_parent = root;
				while (inorder_successor->left) {
					inorder_successor_parent = inorder_successor;
					inorder_successor = inorder_successor->left;
				}

				swap(*inorder_successor, *root);

				if (inorder_successor_parent->right == inorder_successor)
					inorder_successor_parent->right = inorder_successor->right;
				else
					inorder_successor_parent->left = inorder_successor->right;

				delete inorder_successor;
			}
		}

		if (deleted_left && root->left == nullptr)
			return root;
		if (!deleted_left && root->right == nullptr)
			return root;

		size_t my_size = root->size();
		bool i_am_scapegoat = my_size < ceill((deleted_left ? root->left->size() : root->right->size()) * factor);
		if (!i_am_scapegoat)
			return root;

		size_t ind = 0;
		std::vector<Node<Key, Value>*> store(my_size);
		to_array(root, store, ind);

		ind = 0;
		return create_balanced_tree(store, ind, store.size());
	}

	static Node<Key, Value>* create_balanced_tree(std::vector<Node<Key, Value>*> nodes, uint64_t index, uint64_t num_elements) {
		if (index >= nodes.size() || index < 0) {
			//assert(false);
			return nullptr;
		}
		if (num_elements == 0)
			return nullptr;

		uint64_t midpoint = num_elements / 2;
		nodes[index + midpoint]->left = create_balanced_tree(nodes, index, midpoint);
		nodes[index + midpoint]->right = create_balanced_tree(nodes, index + midpoint + 1, num_elements - midpoint - 1);

		return nodes[index + midpoint];
	}

	static size_t log3by2(size_t value) {
		static long double log2offactor = log2l(factor);
		return static_cast<size_t>(std::ceill(log2l((double)value) / log2offactor));
	}

	static void to_array(Node<Key, Value>* r, std::vector<Node<Key, Value>*>& store, size_t& ind) {
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
	constexpr static double factor = 3.0 / 2.0;
};

#endif // !SCAPEGOAT_H