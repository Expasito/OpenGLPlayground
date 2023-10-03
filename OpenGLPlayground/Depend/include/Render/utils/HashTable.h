#pragma once
#include "Render/Headers.h"
#include <Render/Render.h>

// we will use a hash table to store the vectors for each model
// there is no remove function because a single model does not take up much space but it may get added 
// in a future update
class HashTable {


public:
	int size;
	// vector of vectors
	std::vector<Render::object*>** arr;
	HashTable(int size_) {
		size = size_;
		arr = (std::vector<Render::object*>**)malloc(sizeof(std::vector<Render::object*>*) * size);
		for (int i = 0; i < size; i++) {
			arr[i] = new std::vector<Render::object*>;
		}

	}

	// add a string to the hash table
	void add(Render::object* val) {
		int index = _hash(val->name);
		arr[index]->push_back(val);
	}


	// see all values at each index
	void see() {
		for (int i = 0; i < size; i++) {
			std::vector<Render::object*>* temp = arr[i];
			std::cout << i << "(" << temp->size() << ")" << "\n";
			for (int j = 0; j < temp->size(); j++) {
				std::cout << (temp)->at(j)->name << "\n";
			}

			std::cout << "\n";
		}
	}



private:

	// honestly works pretty well
	// just take the character value times the position and sum it up
	int _hash(std::string val) {
		int sum = 0;
		for (int i = 0; i < val.length(); i++) {
			sum += val[i] * (i + 1);
		}


		return sum % size;

	}
};
