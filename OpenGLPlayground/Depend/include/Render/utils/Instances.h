#include "Render/Headers.h"
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Instance {
	bool keep;
	int key;
	glm::vec3 trans;
	glm::vec3 rot;
	glm::vec3 scal;
	glm::vec3 color;
};


class HT {
public:
	Instance* table;
	int size;
	int elements;

	float load;

	HT(int size_) {
		// allocate memory for our table
		table = (Instance*)malloc(sizeof(Instance) * size_);
		size = size_;

		init();
		elements = 0;
		load = 0;
	}

	~HT() {
		free(table);
	}


	// key should be unique from all other keys
	void add(int key, glm::vec3 trans, glm::vec3 rot, glm::vec3 scal, glm::vec3 color) {
		int index = _hash(key);
		int index_ = index;

		while (true) {
			Instance inst = table[index];
			if (inst.keep == true) {
				index++;
				if (index >= size) {
					index = 0;
				}
			}
			else {
				table[index] = { true, key, trans, rot, scal,color };
				break;
			}

			// we have failed to add because no slots are open
			if (index == index_) {
				break;
			}

		}
		elements++;
		load = (float)elements / size;
	}

	void add(Instance inst_) {
		int index = _hash(inst_.key);
		int index_ = index;

		while (true) {
			Instance inst = table[index];
			if (inst.keep == true) {
				index++;
				if (index >= size) {
					index = 0;
				}
			}
			else {
				table[index] = inst_;
				break;
			}

			// we have failed to add because no slots are open
			if (index == index_) {
				break;
			}

		}
		elements++;
		load = (float)elements / size;
	}

	// deletes the instance by marking it as open.
	void remove(int key) {
		int index = _hash(key);
		int index_ = index;

		while (true) {
			Instance inst = table[index];
			if (inst.key != key) {
				index++;
				if (index >= size) {
					index = 0;
				}
			}
			else {
				table[index].keep = false;
				break;
			}

			// we have failed to add because no slots are open
			if (index == index_) {
				break;
			}

		}
		elements--;
		load = (float)elements / size;
	}

	// only call get if the instance is in there
	Instance* get(int key) {
		int index = _hash(key);
		int index_ = index;
		while (true) {
			Instance* inst = &table[index];
			if (inst->key == key) {
				return inst;
			}
			else {
				index++;
				if (index > size) {
					index = 0;
				}
			}
			if (index == index_) {
				return NULL;
			}
		}
	}

	void see() {
		for (int i = 0; i < size; i++) {
			std::cout << i << "\n";
			Instance inst = table[i];
			if (inst.keep) {
				std::cout << "  " << inst.keep << ": " << inst.key << " : (" << inst.trans.x << ", " << inst.trans.y << ", " << inst.trans.z << ")" << "\n";

			}
			else {
				std::cout << "  " << inst.keep << "\n";
			}
		}

		std::cout << "load:" << load << "\n\n\n";
	}

private:

	int _hash(int index) {
		return index % size;
	}


	// set all keep values to false so all slots are open
	void init() {
		for (int i = 0; i < size; i++) {
			table[i].keep = false;
		}
		elements = 0;
	}
};


class Instances {
public:
	HT* table;

	// min number of slots to keep in the table
	int minSlots = 4;

	// this is what to multiply by to resize the table
	int resizeFactor = 2;

	// maxload only applies for adding. Minload only applies for removing
	float maxLoad = .25;
	float minLoad = .125;

	Instances(int size) {
		table = new HT(size);

	}

	void add(int key, glm::vec3 trans, glm::vec3 rot, glm::vec3 scal,glm::vec3 color) {
		table->add(key, trans, rot, scal,color);
		std::cout << "             " << table->load << "\n";
		if (table->load > maxLoad) {
			resize(table->size * resizeFactor);
		}
	}

	void remove(int key) {
		table->remove(key);

		if (table->size / resizeFactor > minSlots && table->load < minLoad) {
			resize(table->size / resizeFactor);
		}
	}

	void resize(int size) {
		//std::cout << "Resizing now:  " << size << "\n";
		HT* temp = new HT(size);

		for (int i = 0; i < table->size; i++) {
			Instance inst = table->table[i];
			if (inst.keep)
				temp->add(inst);
		}


		delete table;

		//std::cout << "new size: " << temp->size << "\n";

		table = temp;
	}

	void see() {
		table->see();
	}




};