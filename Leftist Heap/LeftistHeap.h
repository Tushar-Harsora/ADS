#ifndef LEFTIST_HEAP_H
#define LEFTIST_HEAP_H
#include <iostream>
#include <cassert>

template<typename Key>
struct heap_node {
private:
	constexpr static size_t space_block = 7;
public:
	Key key;
	size_t rank;
	heap_node<Key>* left;
	heap_node<Key>* right;
	heap_node(const Key& key) : key(key), rank(1), left(nullptr), right(nullptr) { }

	friend heap_node<Key>* merge(heap_node<Key>* l, heap_node<Key>* r) {
		if (l == nullptr)
			return r;
		if (r == nullptr)
			return l;
		if (l->key < r->key) {
			l->right = merge(l->right, r);
			assert(l->right);
			if (!l->left || l->left->rank < l->right->rank) {
				std::swap(l->left, l->right);
			}
			l->rank = l->right ? l->right->rank + 1 : 1;
			return l;
		}
		else {
			r->right = merge(r->right, l);
			assert(r->right);
			if (!r->left || r->left->rank < r->right->rank) {
				std::swap(r->left, r->right);
			}
			r->rank = r->right ? r->right->rank + 1 : 1;
			return r;
		}
		assert(false);
	}

	friend std::ostream& print_helper(std::ostream& stream, const heap_node<Key>* ptr, size_t spaces = 0) {
		if (!ptr)
			return stream;
		spaces += space_block;
		
		print_helper(stream, ptr->right, spaces);
		stream << '\n';
		for (int i = 0; i < spaces; i++)
			stream << ' ';
		stream << ptr->key << " (" << ptr->rank << ")";
		print_helper(stream, ptr->left, spaces);
	}

	~heap_node() {
		if (left) {
			delete left;
		}
		if (right) {
			delete right;
		}
	}
};

template<typename Key>
struct leftist_heap {
private:
	heap_node<Key>* m_root;

	heap_node<Key>* _insert(heap_node<Key>* root, const Key& key) {
		if (!root) {
			return new heap_node<Key>(key);
		}

		if (key < root->key) {
			heap_node<Key>* new_node = new heap_node<Key>(key);
			new_node->left = root;
			return new_node;
		}

		if (root->right && key > root->right->key) {
			root->right = _insert(root->right, key);
			assert(root->left && root->right);
			if (root->left->rank < root->right->rank)
				std::swap(root->left, root->right);
			root->rank = root->right->rank + 1;
			return root;
		}

		heap_node<Key>* new_node = new heap_node<Key>(key);
		if (root->left == nullptr) {
			root->left = new_node;
			return root;
		}

		assert(root->right == nullptr);
		assert(root->rank == 1);
		root->right = new_node;
		root->rank = 2;

		assert(root->left && root->right);
		if (root->left->rank < root->right->rank) {
			std::swap(root->left, root->right);
		}

		return root;
	}
public:
	leftist_heap() : m_root(nullptr) { }

	void insert(const Key& key) {
		m_root = _insert(m_root, key);
		// We could also create new heap with one node and merge with existing tree.
		// I wrote this merge function later so i already had insert both implementations are fine
		// auto new_node = new heap_node<Key>(key);
		// m_root = merge(m_root, new_node);
	}

	void pop() {
		heap_node<Key>* left_child = m_root->left;
		heap_node<Key>* right_child = m_root->right;

		m_root->left = nullptr;
		m_root->right = nullptr;
		delete m_root;		// resetting left, right was necessary because delete operator deletes childs recursively

		m_root = merge(left_child, right_child);
	}

	const Key& top() const {
		return m_root->key;
	}

	bool empty() const { return m_root == nullptr; }

	void vertical_print() const {
		print_helper(std::cout, m_root);
	}

	~leftist_heap() {
		delete m_root;
		m_root = nullptr;
	}
};

#endif // LEFTIST_HEAP_H