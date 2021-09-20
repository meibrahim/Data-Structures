// Mazen Ibrahim
// 295924



#include "Dictionary.h"

void printMaxFrequency(const Dictionary<string, int>& d, int max, bool exact) {

}

void printMinFrequency(const Dictionary<string, int>& d, int max, bool exact) {

}

void test_1(Dictionary<string, int>& dictionary);
void test_2(Dictionary<string, int>& dictionary);
void test_3(Dictionary<string, int>& dictionary);
void test_4(Dictionary<string, int>& dictionary);

int main(int argc, char** argv) {
	Dictionary<string, int> dictionary;
	test_1(dictionary);

	return 0;
}

void test_1(Dictionary<string, int>& dictionary) {
	dictionary.insert("hello", 1);
	dictionary.insert("there", 2);
	dictionary.insert("ah", 2);
	dictionary.insert("general", 4);
	dictionary.insert("Kenobi", 5);

	assert(dictionary.remove("ah"));
	assert(dictionary.exists("hello"));
	assert(dictionary.exists("there"));
	assert(dictionary.exists("general"));
	assert(dictionary.exists("Kenobi"));

	test_2(dictionary);
}

void test_2(Dictionary<string, int>& dictionary) {

	Dictionary<string, int> dictionary2 = Dictionary<string, int>(dictionary);

	dictionary.remove_all();
	assert(dictionary.size() == 0);

	dictionary.insert("general", 4);
	dictionary.insert("Kenobi", 5);
	assert(dictionary.size() == 2);

	test_3(dictionary2);
}

void test_3(Dictionary<string, int>& dictionary2) {
	assert(dictionary2.exists("hello"));
	assert(dictionary2.exists("there"));
	assert(dictionary2.exists("general"));
	assert(dictionary2.exists("Kenobi"));
	assert(dictionary2.size() == 4);

	assert(dictionary2.remove("there"));
	assert(!dictionary2.exists("there"));

	test_4(dictionary2);
}

void test_4(Dictionary<string, int>& dictionary) {
	dictionary.insert("so", 15);
	dictionary.insert("uncivilized", 16);
	dictionary.insert("another", 16);
	dictionary.insert("happy", 22);
	dictionary.insert("landing", 7);

	dictionary.traverse_in();
}