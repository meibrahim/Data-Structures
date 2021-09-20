// Mazen Ibrahim
// 295924

#include "Sequence.h"


int main(void) {

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
	B.push_back(70, 70);
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
}