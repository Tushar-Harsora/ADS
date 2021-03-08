#pragma once

#include <algorithm>
#include <stack>
#include <iostream>
#include <cstdint>
#include <cassert>
using namespace std;

template<typename Key, typename Value>
struct Node {
public:
	Node() = default;
	Node(Key key, Value value, int64_t height) : key(key), value(value), height(height),
		left(nullptr), right(nullptr) { }

	Key key;
	Value value;
	int64_t height;
	Node<Key, Value>* left;
	Node<Key, Value>* right;

	friend ostream& operator<<(ostream& out, const Node<Key, Value>* const node) {
		if (node == nullptr)
			return out << "\n";
		// Or Instead You Can Prematurely Check if left or right child exists
		// if yes then call operator<< recursively
		out << node->left;
		out << node->key << " : " << node->value << " Height : " << node->height;
		out << node->right;

		return out;
	}

	friend void swap(Node<Key, Value>& lhs, Node<Key, Value>& rhs) {
		using std::swap;
		swap(lhs.key, rhs.key);
		swap(lhs.value, rhs.value);
	}
};

template<typename Key, typename Value>
struct AVL {
public:
	Node<Key, Value>* root;
private:
	static int64_t getHeight(Node<Key, Value>* node) {
		return node == nullptr ? 0 : node->height;
	}

	static Node<Key, Value>* _right_rotate(Node<Key, Value>* x) {
		/*
						 x                                   y
						/ \                                 / \
					   y   xr         ==>                  /   \
					  / \                                 z     x
					 z   yr                              / \   / \
					/ \                                 zl zr yr xr
				   zl  zr
		*/
		auto y = x->left;
		auto yr = x->left->right;

		y->right = x;
		x->left = yr;

		x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));

		y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));

		return y;
	}

	static Node<Key, Value>* _left_rotate(Node<Key, Value>* x) {
		/*
						 x                                    y
						/ \                                  / \
					  xl   y          ==>                   /   \
						  / \                              x     z
						 yl  z                            / \   / \
							/ \                          xl yl zl zr
						   zl  zr
		*/
		auto y = x->right;
		auto yl = x->right->left;

		y->left = x;
		x->right = yl;

		x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
		y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));

		return y;
	}

	Node<Key, Value>* _insert(Node<Key, Value>* root, Node<Key, Value>* new_node) {
		if (root == nullptr)
			return new_node;

		if (new_node->key < root->key)						// If Key Compares Less than then Left Subtree
			root->left = _insert(root->left, new_node);
		else
			root->right = _insert(root->right, new_node);	// If Key Compares greater equal then Right Subtree

		root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

		auto balance = getHeight(root->left) - getHeight(root->right);

		/*
		* There Are 4 cases That are possible
		* 1) Height of left Subtree is Greater
		*		a) key of grand child is less than key of child				(Right Rotate)
		*		b) key of grand child is greater equal to key of child		(Left Right Rotate)
		* 2) Height of right Subtree is Greater
		*		a) key of grand child is less than key of child				(Right Left Rotate)
		*		b) key of grand child is greater equal to key of child		(Left Rotate)
		*/

		if (balance > 1) {								// Height Of left subtree is greater
			if (new_node->key < root->left->key) {		// Right Rotate
				return _right_rotate(root);
			}
			else {										// Left Right Rotate
				root->left = _left_rotate(root->left);
				return _right_rotate(root);
			}
		}
		else if (balance < -1) {						// Height Of right subtree is greater
			if (new_node->key < root->right->key) {		// Right Left Rotate
				root->right = _right_rotate(root->right);
				return _left_rotate(root);
			}
			else {										// Left Rotate
				return _left_rotate(root);
			}
		}

		// There Was No Balancing Issues return this node
		return root;
	}

	Node<Key, Value>* _delete(Node<Key, Value>* root, const Key& key) {
		if (root == nullptr)
			return root;

		if (key < root->key) {
			root->left = _delete(root->left, key);
		}
		else if (key > root->key) {
			root->right = _delete(root->right, key);
		}
		else {		// Delete This
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
					inorder_successor_parent->right = nullptr;
				else
					inorder_successor_parent->left = nullptr;

				delete inorder_successor;
			}
		}

		auto leftHeight = getHeight(root->left);
		auto rightHeight = getHeight(root->right);
		root->height = max(leftHeight, rightHeight) + 1;

		auto balance = get_balance(root);

		if (balance > 1) {
			auto left_balance = get_balance(root->left);
			if (left_balance >= 0) {
				return _right_rotate(root);
			}
			else {
				root->left = _left_rotate(root->left);
				return _right_rotate(root);
			}
		}
		else if (balance < -1) {
			auto right_balance = get_balance(root->right);
			if (right_balance <= 0) {
				return _left_rotate(root);
			}
			else {
				root->right = _right_rotate(root->right);
				return _left_rotate(root);
			}
		}

		return root;
	}

	static auto get_balance(Node<Key, Value>* root) {
		return getHeight(root->left) - getHeight(root->right);
	}

public:
	AVL() : root(nullptr) { }

	void insert(const Key& key, const Value& value) {
		Node<Key, Value>* node = new Node<Key, Value>(key, value, 1);
		root = _insert(root, node);
	}

	void erase(const Key& key) {
		root = _delete(root, key);
	}

	friend Node<Key, Value>* join(AVL<Key, Value> lhs, AVL<Key, Value> rhs) {
		// Make Sure Tree in lhs has every key less than smallest key in rhs
		auto minlhs = lhs.root;
		auto maxlhs = lhs.root;
		auto minrhs = rhs.root;
		auto maxrhs = rhs.root;
		while (minlhs->left)	minlhs = minlhs->left;
		while (maxlhs->right)	maxlhs = maxlhs->right;
		while (minrhs->left)	minrhs = minrhs->left;
		while (maxrhs->right)	maxrhs = maxrhs->right;
#ifdef _DEBUG
		assert(maxlhs->key < minrhs->key);
		if (lhs.root == rhs.root)
			assert(false);
#endif // _DEBUG
		auto rebalance = [](Node<Key, Value>* root) -> Node<Key, Value>*{
			auto leftHeight = getHeight(root->left);
			auto rightHeight = getHeight(root->right);
			root->height = max(leftHeight, rightHeight) + 1;

			auto balance = get_balance(root);

			if (balance > 1) {
				auto left_balance = get_balance(root->left);
				if (left_balance >= 0) {
					return _right_rotate(root);
				}
				else {
					root->left = _left_rotate(root->left);
					return _right_rotate(root);
				}
			}
			else if (balance < -1) {
				auto right_balance = get_balance(root->right);
				if (right_balance <= 0) {
					return _left_rotate(root);
				}
				else {
					root->right = _right_rotate(root->right);
					return _left_rotate(root);
				}
			}
			else {
				return root;
			}
		};
		
		auto lhs_height = lhs.root->height, rhs_height = rhs.root->height;
		if (abs(lhs_height - rhs_height) <= 1) {		// Case 1
			Node<Key, Value>* new_root = new Node<Key, Value>(minrhs->key, minrhs->value, 1);	// TODO: set valid height
			rhs.erase(minrhs->key);
			new_root->left = lhs.root;
			new_root->right = rhs.root;
			new_root->height = 1 + max(getHeight(new_root->left), getHeight(new_root->right));
			return new_root;
		}
		else if(lhs_height <= rhs_height - 2) {		// Case 2 And 3 Because we are sure height of left tree is smaller
			Node<Key, Value>* new_root = new Node<Key, Value>(minrhs->key, minrhs->value, 1);	// TODO: set valid height
			rhs.erase(minrhs->key);

			lhs_height = getHeight(lhs.root), rhs_height = getHeight(rhs.root);
			Node<Key, Value>* ptr = rhs.root;
			stack<Node<Key, Value>*> stck;
			while (getHeight(ptr) > lhs_height) {
				stck.push(ptr);
				ptr = ptr->left;
			}

			//TODO: Stack will not always have top element
			if (getHeight(ptr) == lhs_height && stck.top()->height == lhs_height + 2) {		// Case 2.1 OR 3.1
				new_root->height = lhs_height + 1;
				stck.top()->left = new_root;
				new_root->left = lhs.root;
				new_root->right = ptr;
			}
			else if (getHeight(ptr) == lhs_height && stck.top()->height == lhs_height + 1) {
				new_root->height = lhs_height + 1;
				stck.top()->left = new_root;
				new_root->left = lhs.root;
				new_root->right = ptr;
				while (!stck.empty()) {
					stck.top() = rebalance(stck.top());
					if (stck.size() == 1)
						rhs.root = stck.top();
					stck.pop();
				}
			}
			else if (getHeight(ptr) == lhs_height - 1 && stck.top()->height == lhs_height + 1) {
				new_root->height = lhs_height + 1;
				stck.top()->left = new_root;
				new_root->left = lhs.root;
				new_root->right = ptr;
				while (!stck.empty()) {
					stck.top() = rebalance(stck.top());
					if (stck.size() == 1)
						rhs.root = stck.top();
					stck.pop();
				}
			}
			else {
				assert(false);
			}
			return rhs.root;
		}
		else if (rhs_height <= lhs_height - 2) {
			Node<Key, Value>* new_root = new Node<Key, Value>(minrhs->key, minrhs->value, 1);
			rhs.erase(minrhs->key);

			lhs_height = getHeight(lhs.root), rhs_height = getHeight(rhs.root);
			Node<Key, Value>* ptr = lhs.root;
			stack<Node<Key, Value>*> stck;
			while (getHeight(ptr) > rhs_height) {
				stck.push(ptr);
				ptr = ptr->right;
			}

			if (getHeight(ptr) == rhs_height && stck.top()->height == rhs_height + 2) {
				new_root->height = rhs_height + 1;
				stck.top()->right = new_root;
				new_root->left = ptr;
				new_root->right = rhs.root;
			}
			else if (getHeight(ptr) == rhs_height && stck.top()->height == rhs_height + 1) {
				new_root->height = rhs_height + 1;
				stck.top()->right = new_root;
				new_root->left = ptr;
				new_root->right = rhs.root;
				// Root change thai che!!
				while (!stck.empty()) {
					stck.top() = rebalance(stck.top());
					if (stck.size() == 1)
						lhs.root = stck.top();
					stck.pop();
				}
			}
			else if (getHeight(ptr) == rhs_height - 1 && stck.top()->height == rhs_height + 1) {
				new_root->height = rhs_height + 1;
				stck.top()->right = new_root;
				new_root->left = ptr;
				new_root->right = rhs.root;
				while (!stck.empty()) {
					stck.top() = rebalance(stck.top());
					if (stck.size() == 1)
						lhs.root = stck.top();
					stck.pop();
				}
			}
			else {
				assert(false);
			}
			return lhs.root;
		}
		else {
			assert(false);
		}
	}

	friend ostream& operator<<(ostream& out, const AVL<Key, Value>& avl) {
		out << avl.root;
		return out;
	}
};