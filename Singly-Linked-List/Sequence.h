// Mazen Ibrahim
//295924

#include <iostream>
#include "stdint.h"

typedef int32_t index_t;

template <typename TKey, typename TInfo>
class Sequence {
public:
	class Node;
	class Iterator;  // an iterator class to iterate through our sequence list and all the members of this class will have an access to the members of class sequence 

	Sequence(void) : head_m(nullptr) { } // just a default constructor 

	Sequence(const Sequence& copy_t) : head_m(nullptr) {  // a copy constructor to copy the elements of a sequence 
		copy(copy_t, 0, copy_t.count());
	}

	Sequence(Sequence&& move) noexcept : head_m(nullptr) {  // a move constructor 
		std::swap(head_m, move.head_m);
	}

	~Sequence(void) { // destructor when invoked all the elements in the sequence will be destroyed 
		clear();
	}

	TKey key(const index_t index) const; // to be able to read a key at specified index
	TInfo& info(const index_t index);  // to be able to edit the value of the key at specified index 

	Iterator end(void) const;  
	Iterator begin(void) const;
	Iterator at(const index_t index) const;
	Iterator skip(const size_t count) const;
	Iterator find(const TKey& key, const size_t occurrence = 1) const;

	index_t index_of(const Iterator& iter) const;
	index_t index_of(const TKey& key, const size_t occurrence = 1) const;

	bool is_empty(void) const; // to check if our sequence list is empty
	bool contains(const TKey& key) const; // to check if our sequence contains a certain key 

	size_t count(void) const; // just to count the number of elements in our sequence list 
	size_t count(const TKey& key) const; // to count the number of the occurances of a certain key

	size_t remove_all(const TKey& key, const index_t offset = 0); // to remove all occurances of a certain key

	void clear(void); // delete all elements in the sequence 
	void remove_at(const index_t index); // delete a specific node at specific index 

	void copy(const Sequence& from, const index_t start, const size_t count); // a copy method 
	void insert(const TKey& key, const TInfo& info, const index_t index);	// insertion method at which we provide the key and the info and the index we would like to insert it in
	void push_front(const TKey& key, const TInfo& info); // add element at the beginning 
	void push_back(const TKey& key, const TInfo& info); // add element at the end 
	bool pop_front(void); // remove element from beginning 
	bool pop_back(void);  // remove element from the end 

	void print(std::ostream& os) const; // overloading print 

	Sequence& operator=(const Sequence& other) { //overloading assignment operation 
		if (this != *other) {
			clear();
			copy(other, 0, other.count());
		}
		return *this;
	}

	Sequence operator+(const Sequence& other) {  // overloading adding operation 
		Sequence result(*this);
		result.copy(other, 0, other.count());
		return result;
	}

	friend std::ostream& operator<<(std::ostream& os, const Sequence<TKey, TInfo>& seq); 

	class Iterator {
	public:
		Iterator(const Iterator& copy)
			: node_m(copy.node_m) { }

		Iterator(Iterator&& move) noexcept : node_m(nullptr) {
			std::swap(node_m, move.node_m);
		}

		Node* operator *(void) const {
			return this->node_m;
		}

		Node* operator ->(void) const {
			return this->node_m;
		}

		Iterator& operator =(const Iterator& value) {
			if (this != &value) {
				node_m = value.node_m;
			}
			return *this;
		}

		Iterator& operator ++(void) {
			if (this->node_m != nullptr) {
				this->node_m = this->node_m->next_m;
			}
			return *this;
		}

		Iterator operator ++(int) {
			auto prev = *this;
			++* this;
			return prev;
		}

		bool operator != (const Iterator& iter) {
			return this->node_m != iter.node_m;
		}

		bool operator == (const Iterator& iter) {
			return this->node_m == iter.node_m;
		}

	private:
		Iterator(Node* node_t) : node_m(node_t) { }

		Node* node_m;
		friend class Sequence<TKey, TInfo>;
	};

	class Node {
	public:
		Node(const TKey& key, const TInfo& info, Node* next = nullptr)
			: key_m(key), info_m(info), next_m(next) { }

		Node(const Node& copy) 
			: key_m(copy.key_m), info_m(copy.info_m), next_m(copy.next_m) { }

		Node(Node&& move) noexcept : key_m(), info_m(), next_m(nullptr) {
			std::swap(key_m, move.key_m);
			std::swap(info_m, move.info_m);
			std::swap(next_m, move.next_m);
		}

		TKey key(void) const { 
			return key_m; 
		}

		Node* next(void) const {
			return next_m;
		}

		TInfo& info(void) { 
			return info_m; 
		}

		bool operator == (const Node& node) {
			return this->key == node.key
				&& this->info == node.info;
		}

		bool operator != (const Node& node) {
			return this->key != node.key
				|| this->info != node.info;
		}

	private:
		TKey key_m;
		TInfo info_m;
		Node* next_m;

		~Node(void) { 
			if (next_m != nullptr) {
				delete next_m;
			}
		}

		friend class Sequence<TKey, TInfo>;
	};

private:
	//  to make sure we don't lose anything, or accidently destroy the whole sequence
	void remove_assert(Node* prev, Node* node) {
		if (head_m == node) {
			head_m = head_m->next_m;
		}
		else {
			prev->next_m = node->next_m;
		}
		node->next_m = nullptr;
		delete node;
	}

	
	Node* node_at(const index_t index, Node*& prev) const {
		if (index < 0) {
			return nullptr;
		}
		prev = nullptr;
		auto result = head_m;
		for (auto i = 0; i < index; i++) {
			prev = result;
			result = result->next_m;
		}
		return result;
	}
	Node* node_at(const index_t index) const {
		Node* prev;
		return node_at(index, prev);
	}
	//

	
	Node* tail(Node*& prev) const {
		prev = nullptr;
		if (head_m == nullptr) {
			return nullptr;
		}
		auto result = head_m;
		while (result->next_m != nullptr) {
			prev = result;
			result = result->next_m;
		}
		return result;
	}
	Node* tail(void) const {
		Node* prev;
		return tail(prev);
	}
	//

	// our main lookup method
	bool find_node(const TKey& key, const size_t occurrence, index_t& index, Node*& node, Node*& prev) const {
		index = 0;
		auto counter = 0;
		for (auto iter = begin(); iter != end(); ++iter, ++index) {
			if (iter->key_m == key) {
				if (++counter == occurrence) {
					node = *iter;
					return true;
				}
			}
			prev = *iter;
		}
		return false;
	}
	// and just helpers for the method above
	bool find_node(const TKey& key, const size_t occurrence, index_t& index, Node*& node) const {
		Node* prev;
		return find_node(key, occurrence, index, node, prev);
	}
	bool find_node(const TKey& key, const size_t occurrence, Node*& node) const {
		Node* prev;
		index_t index;
		return find_node(key, occurrence, index, node, prev);
	}
	//


	//
	Node* head_m;
};


template <typename TKey, typename TInfo>
TKey Sequence<TKey, TInfo>::key(const index_t index) const {
	return node_at(index)->key();
}

template <typename TKey, typename TInfo>
TInfo& Sequence<TKey, TInfo>::info(const index_t index) {
	return node_at(index)->info();
}

template <typename TKey, typename TInfo>
typename Sequence<TKey, TInfo>::Iterator Sequence<TKey, TInfo>::end(void) const {
	return Iterator(nullptr);
}

template <typename TKey, typename TInfo>
typename Sequence<TKey, TInfo>::Iterator Sequence<TKey, TInfo>::begin(void) const {
	return Iterator(head_m);
}

template <typename TKey, typename TInfo>
typename Sequence<TKey, TInfo>::Iterator Sequence<TKey, TInfo>::at(const index_t index) const {
	return Iterator(node_at(index));
}

template <typename TKey, typename TInfo>
typename Sequence<TKey, TInfo>::Iterator Sequence<TKey, TInfo>::skip(const size_t count) const {
	return Iterator(node_at(count + 1));
}

template <typename TKey, typename TInfo>
typename Sequence<TKey, TInfo>::Iterator Sequence<TKey, TInfo>::find(const TKey& key, const size_t occurrence) const {
	Node* node;
	if (find_node(key, occurrence, node)) {
		return Iterator(node);
	}
	else {
		return end();
	}
}

template <typename TKey, typename TInfo>
index_t Sequence<TKey, TInfo>::index_of(const Iterator& i) const {
	index_t result = 0;
	for (auto iter = begin(); iter != i; iter++, result++) {
		if (iter == end()) {
			return -1;
		}
	}
	return result;
}

template <typename TKey, typename TInfo>
index_t Sequence<TKey, TInfo>::index_of(const TKey& key, const size_t occurrence) const {
	Node* node;
	index_t result;
	if (find_node(key, occurrence, result, node)) {
		return result;
	} 
	return -1;
}

template <typename TKey, typename TInfo>
bool Sequence<TKey, TInfo>::is_empty(void) const {
	return head_m == nullptr;
}

template <typename TKey, typename TInfo>
bool Sequence<TKey, TInfo>::contains(const TKey& key) const {
	return index_of(key, 1) > -1;
}

template <typename TKey, typename TInfo>
size_t Sequence<TKey, TInfo>::count(void) const {
	size_t result = 0;
	for (auto iter = begin(); iter != end(); iter++, result++);
	return result;
}

template <typename TKey, typename TInfo>
size_t Sequence<TKey, TInfo>::count(const TKey& key) const {
	size_t result = 0;
	for (auto iter = begin(); iter != end(); iter++) {
		if (iter->key_m == key) {
			++result;
		}
	}
	return result;
}

template <typename TKey, typename TInfo>
void Sequence<TKey, TInfo>::clear(void) {
	if (head_m != nullptr) {
		delete head_m;
		head_m = nullptr;
	}
}

template <typename TKey, typename TInfo>
size_t Sequence<TKey, TInfo>::remove_all(const TKey& key, const index_t offset) {
	size_t result = 0;
	Node* prev = nullptr;
	Node* iter = node_at(offset, prev);

	while (iter != nullptr) {
		if (iter->key_m == key) {
			remove_assert(prev, iter);
			iter = prev;
			++result;
		}
		prev = iter;
		iter = iter->next_m;
	}

	return result;
}

template <typename TKey, typename TInfo>
void Sequence<TKey, TInfo>::remove_at(const index_t index) {
	Node* prev = nullptr;
	Node* iter = node_at(index, prev);
	remove_assert(prev, iter);
}

template <typename TKey, typename TInfo>
void Sequence<TKey, TInfo>::copy(const Sequence<TKey, TInfo>& from, const index_t start, const size_t count) {
	auto remaining = count;
	for (auto iter = from.at(start); remaining > 0; iter++, remaining--) {
		push_front(iter->key_m, iter->info_m);
	}
}

template <typename TKey, typename TInfo>
void Sequence<TKey, TInfo>::insert(const TKey& key, const TInfo& info, const index_t index) {
	if (index < 0 || (size_t)index > count()) {
		throw "Index out of range";
	}

	auto iter = node_at(index - 1);
	if (iter == nullptr) {
		head_m = new Node(key, info, head_m->next_m);
	} 
	else {
		iter->next_m = new Node(key, info, iter->next_m);
	}
}

template <typename TKey, typename TInfo>
void Sequence<TKey, TInfo>::push_front(const TKey& key, const TInfo& info) {
	if (head_m == nullptr) {
		head_m = new Node(key, info);
	} 
	else{
		tail()->next_m = new Node(key, info);
	}
}

template <typename TKey, typename TInfo>
void Sequence<TKey, TInfo>::push_back(const TKey& key, const TInfo& info) {
	if (head_m == nullptr) {
		head_m = new Node(key, info);
	} else {
		head_m = new Node(key, info, head_m);
	}
}

template <typename TKey, typename TInfo>
bool Sequence<TKey, TInfo>::pop_front(void) {
	if (head_m != nullptr) {
		Node* prev;
		Node* node = tail(prev);
		remove_assert(prev, node);
		return true;
	}
	return false;
}

template <typename TKey, typename TInfo>
bool Sequence<TKey, TInfo>::pop_back(void) {
	if (head_m != nullptr) {
		remove_assert(nullptr, head_m);
		return true;
	}
	return false;
}

template <typename TKey, typename TInfo>
void Sequence<TKey, TInfo>::print(std::ostream& os) const {
	for (auto iter = begin(); iter != end(); iter++) {
		os << "{ Key: " << iter->key_m << " : Info: " << iter->info_m << " }" << std::endl;
	}
}

template <typename TKey, typename TInfo>
std::ostream& operator<<(std::ostream& os, const Sequence<TKey, TInfo>& seq) {
	seq.print(os);
}

template < typename Key, typename Info>
void split(const Sequence<Key, Info>& source, const Key& keyStart, int keyOccurance, int total, Sequence<Key, Info>& target1, int length1, Sequence<Key, Info>& target2, int length2) {
	auto limit = total;
	for (auto iter = source.find(keyStart, keyOccurance); iter != source.end();) {

		for (auto i = std::min(limit, length1); i > 0; --i, iter++) {
			target1.push_front(iter->key(), iter->info());
		}

		if ((limit -= length1) <= 0) {
			break;
		}

		for (auto i = std::min(limit, length2); i > 0; --i, iter++) {
			target2.push_front(iter->key(), iter->info());
		}

		if ((limit -= length2) <= 0) {
			break;
		}
	}
}

/*int main(void) {

	Sequence<int, int> A;
	A.push_front(0, 1337);
	A.push_front(228, 1488);
	A.push_front(1111111, 3333352);
	A.push_front(325, 56);
	A.push_front(225, 33);

	std::cout << "A (lots of push_front): " << std::endl;
	A.print(std::cout);
	std::cout << std::endl;

	std::cout << "A.remove_at(4)" << std::endl;
	A.remove_at(4);
	A.print(std::cout);
	std::cout << std::endl;

	Sequence<int, int> B(A);
	std::cout << "B (copy ctor): " << std::endl;
	B.print(std::cout);
	std::cout << std::endl;

	std::cout << "B.copy(A, 1, 2): " << std::endl;
	std::cout << "// should add: {228, 1488} and {1111111, 3333352} again, at the end" << std::endl;
	B.copy(A, 1, 2);
	B.print(std::cout);
	std::cout << std::endl;

	std::cout << "B.remove_all(228, 0): " << std::endl;
	B.remove_all(228, 0);
	B.print(std::cout);
	std::cout << std::endl;

	std::cout << "B.insert(666, 666, 3): " << std::endl;
	B.insert(666, 666, 3);
	B.print(std::cout);
	std::cout << std::endl;

	std::cout << "B.pop_back(): " << std::endl;
	B.pop_back();
	B.print(std::cout);
	std::cout << std::endl;

	std::cout << "B.pop_front(): " << std::endl;
	B.pop_front();
	B.print(std::cout);
	std::cout << std::endl;

	std::cout << "B.push_back(70, 70): " << std::endl;
	B.push_back(696969, 69);
	B.print(std::cout);
	std::cout << std::endl;

	Sequence<int, int> C;
	C.push_front(1, 1);
	C.push_front(2, 2);
	C.push_front(3, 3);
	C.push_front(4, 4);
	C.push_front(5, 5);
	C.push_front(6, 6);
	C.push_front(11, 11);
	C.push_front(1, 1);
	C.push_front(2, 2);
	C.push_front(3, 3);
	C.push_front(4, 4);
	C.push_front(5, 5);
	C.push_front(6, 6);
	C.push_front(7, 7);
	C.push_front(8, 8);
	C.push_front(9, 9);
	C.push_front(10, 10);
	C.push_front(11, 11);
	C.push_front(12, 12);
	C.push_front(13, 13);

	A.clear();
	B.clear();

	std::cout << "split(C, 1, 2, 11, A, 3, B, 2): " << std::endl;
	split(C, 1, 2, 11, A, 3, B, 2);
	std::cout << "A:" << std::endl;
	A.print(std::cout);
	std::cout << std::endl; 
	std::cout << "B:" << std::endl;
	B.print(std::cout);
	std::cout << std::endl; 
	std::cout << "C:" << std::endl;
	C.print(std::cout);
	std::cout << std::endl;

	return 0;
}*/