#pragma once
#include "Render/Headers.h"


class MinHeap {
public:
	struct Element {
		int key;
		int value;
	};

	Element* arr;
	int elements;
	int size;

	MinHeap(int size_) {
		arr = (Element*)malloc(sizeof(Element) * size_);
		elements = 1;
		size = size_;

		for (int i = 0; i < size; i++) {
			arr[i] = { 0,0 };
		}

	}

	int parent(int key) {
		return key / 2;
	}

	int left(int key) {
		return key * 2;
	}

	int right(int key) {
		return key * 2 + 1;
	}

	int compare(int key, int key2) {
		return key < key2;
	}

	void add(int key, int value) {
		std::cout << "key added: " << key << "\n";
		arr[elements] = { key,value };
		bubbleup(elements);
		elements++;
		//std::cout << "see here\n";
		see();
		//std::cout << "end see\n";
	}

	Element remove() {
		Element out = arr[1];
		//std::cout << "key: " << arr[elements - 1].key << "\n";
		arr[1] = arr[elements - 1];
		//std::cout << elements << "\n";
		//std::cout << "herererererer\n";
		//std::cout << "sfjsfjskfjs;lfsf\n";
		bubbledown(1);
		elements--;
		see();
		return out;
	}
	Element remove(int key) {

	}

	void bubbledown(int index) {
		while (right(index) < elements) {
			int leftkey = arr[left(index)].key;
			int rightkey = arr[right(index)].key;
			int larger = leftkey > rightkey ? rightkey : leftkey;
			int dir = leftkey > rightkey ? right(index) : left(index);
			//std::cout << "   index: " << index << " new: " << dir << "\n";
			//std::cout << "   indexK: " << arr[index].key << " newK: " << arr[dir].key << "\n";
			if (larger < arr[index].key) {
				// flip the two and repeat
				Element temp = arr[dir];
				arr[dir] = arr[index];
				arr[index] = temp;
				index = dir;
			}
			else {
				break;
			}
			//std::cout << "seeing now\n";
			//see();
			//std::cout << "stop seeing\n";

		}

	}

	void bubbleup(int index) {
		//std::cout << "         indexes:(Parent,index) " << parent(index) << " " << index << "\n";
		//std::cout << "            keys:{Parent,index) " << arr[parent(index)].key << " " << arr[index].key << "\n";
		if (index > 1 && arr[parent(index)].key > arr[index].key) {
			//std::cout << "     flipping\n";
			// flip the two elements
			Element temp = arr[parent(index)];
			arr[parent(index)] = arr[index];
			arr[index] = temp;
			//index = parent(index);
			bubbleup(parent(index));
		}
	}

	void see() {
		std::cout << "list: ";
		for (int i = 1; i < elements; i++) {
			std::cout << arr[i].key << " ";
		}
		std::cout << "\n";
	}





private:


};

