// Mazen Ibrahim
// 295924


#pragma once
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
using namespace std;

template<typename Key, typename Info>
class Dictionary {
	struct AVL {
	public:
		AVL() : key(), info(), node_l(), node_r() { }
		AVL(const Key& k, const Info& i, AVL* l, AVL* r)
			: key(k), info(i), node_l(l), node_r(r) { }

		Key key;
		Info info;
		AVL* node_l;
		AVL* node_r;
	};
	AVL* tree;

	int get_size(AVL* node) const {
		if (node) {
			return (1
				+ get_size(node->node_l)
				+ get_size(node->node_r)
				);
		}
		return 0;
	}

	int get_balance(AVL* node) const {
		return get_height(node->node_l) - get_height(node->node_r);
	}

	void get_keys(AVL* node, vector<Key>& keys) const {
		if (node) {
			keys.push_back(node->key);
			get_keys(node->node_l, keys);
			get_keys(node->node_r, keys);
		}
	}

	AVL* copy_from(AVL* from) {
		if (from == nullptr) {
			return nullptr;
		}
		else {
			AVL* result = new AVL();
			*result = *from;
			if (result->node_l) {
				result->node_l = copy_from(result->node_l);
			}
			if (result->node_r) {
				result->node_r = copy_from(result->node_r);
			}
			return result;
		}
	}

	AVL* min_key(AVL* node) {
		AVL* min = node;
		while (min->node_l != nullptr) {
			min = min->node_l;
		}
		return min;
	}

	AVL* destroy(AVL* node) {
		if (node) {
			destroy(node->node_l);
			destroy(node->node_r);
			delete node;
			node = nullptr;
		}
		return node;
	}

	AVL* remove(AVL* node, const Key& k, bool& result) {
		if (node) {
			if (k == node->key) {
				result = true;
				if (node->node_l && node->node_r) {
					AVL* smallest = min_key(node->node_r);
					node->key = smallest->key;
					node->info = smallest->info;
					node->node_r = remove(node->node_r, smallest->key, result);
				}
				else {
					if (node->node_l) {
						*node = *(node->node_l);
					}
					if (node->node_r) {
						*node = *(node->node_r);
					}
					delete node;
					node = nullptr;
				}
				return node;
			}
			else {
				if (k > node->key) {
					node->node_r = remove(node->node_r, k, result);
				}
				else {
					node->node_l = remove(node->node_l, k, result);
				}

				node = rotate(node);

				return node;
			}
		}
		else {
			result = false;
			return node;
		}
	}

	AVL* insert(AVL* node, const Key& k, const Info& i, bool& result) {
		if (node) {
			if (k == node->key) {
				result = false;
			}
			else {
				if (k > node->key) {
					node->node_r = insert(node->node_r, k, i, result);
				}
				else {
					node->node_l = insert(node->node_l, k, i, result);
				}

				node = rotate(node);
			}
			return node;
		}
		else {
			return new AVL(k, i, nullptr, nullptr);
		}
	}

	AVL* lookup(AVL* node, const Key& k, bool& result) {
		result = false;
		if (node) {
			if (k == node->key) {
				result = true;
			}
			else {
				if (k > node->key) {
					return lookup(node->node_r, k, result);
				}
				else {
					return lookup(node->node_l, k, result);
				}
			}
		}
		return node;
	}

	AVL* rotate(AVL* parent) {
		int factor = get_balance(parent);
		if (factor == 0) {
			return parent;
		}
		else if (factor < -1) {
			if (get_balance(parent->node_r) <= 0) {
				return rotate_rr(parent);
			}
			else {
				return rotate_rl(parent);
			}
		}
		else if (factor > 1) {
			if (get_balance(parent->node_l) <= 0) {
				return rotate_lr(parent);
			}
			else {
				return rotate_ll(parent);
			}
		}
		return parent;
	}

	AVL* rotate_ll(AVL* parent) {
		AVL* rotate = parent->node_l;
		parent->node_l = rotate->node_r;
		rotate->node_r = parent;
		return rotate;
	}

	AVL* rotate_lr(AVL* parent) {
		AVL* rotate = parent->node_l;
		parent->node_l = rotate_rr(rotate);
		return rotate_ll(parent);
	}

	AVL* rotate_rr(AVL* parent) {
		AVL* rotate = parent->node_r;
		parent->node_r = rotate->node_l;
		rotate->node_l = parent;
		return rotate;
	}

	AVL* rotate_rl(AVL* parent) {
		AVL* rotate = parent->node_r;
		parent->node_r = rotate_ll(rotate);
		return rotate_rr(parent);
	}

	int max(int a, int b) const {
		return a > b ? a : b;
	}

	int get_height(AVL* node) const {
		if (node) {
			return 1 + max(
				get_height(node->node_l),
				get_height(node->node_r)
			);
		}
		return 0;
	}

	void traverse_in(AVL* node) const {
		if (node) {
			traverse_in(node->node_l);
			cout << node->key << " ";
			traverse_in(node->node_r);
		}
	}

	void traverse_pre(AVL* node) const {
		if (node) {
			cout << node->key << " ";
			traverse_pre(node->node_l);
			traverse_pre(node->node_r);
		}
	}

	void traverse_post(AVL* node) const {
		if (node) {
			traverse_post(node->node_l);
			traverse_post(node->node_r);
			cout << node->key << " ";
		}
	}

public:
	Dictionary() : tree(nullptr) {

	}

	Dictionary(const Dictionary& copy) : tree(nullptr) {
		tree = copy_from(copy.tree);
	}

	~Dictionary() {
		remove_all();
	}

	int size() const {
		return get_size(tree);
	}

	void insert(const Key& key, const Info& info) {
		bool result;
		tree = insert(tree, key, info, result);
		if (!result) {
			throw new runtime_error("key already present!");
		}
	}

	void remove_all() {
		tree = destroy(tree);
	}

	bool remove(const Key& key) {
		bool result;
		tree = remove(tree, key, result);
		return result;
	}

	bool is_empty() const {
		return tree == nullptr;
	}

	bool exists(const Key& key) {
		bool result;
		lookup(tree, key, result);
		return result;
	}

	Info& get_info(const Key& key) {
		bool result;
		AVL* node = lookup(tree, key, result);
		if (result) {
			return node->info;
		}
		else {
			throw new runtime_error("key not found!");
		}
	}

	void traverse_in() const {
		traverse_in(tree);
	}

	void traverse_pre() const {
		traverse_pre(tree);
	}

	void traverse_post() const {
		traverse_post(tree);
	}
};

void printMaxFrequency(const Dictionary<string, int>& d, int max, bool exact);
void printMinFrequency(const Dictionary<string, int>& d, int max, bool exact);