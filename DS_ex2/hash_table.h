#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <iostream>
#include "emplyee.h"
#include "linked_list.h"

class HashTable {
private:
	int size;
	int counter;
	LinkedList** employees; //array of LinkedList, the value of every index of the array is a linked list of employees 
	static const int init_size = 10;

public:
	HashTable();
	HashTable (const HashTable& hash_table) = default;
	HashTable& operator=(const HashTable& node) = default;
	~HashTable();

	void resize(int new_size, LinkedList* old_employees_array[], int old_size);
	int HashFunction(int employee_id);
	Employee* find(int employee_id);

	bool insert(Employee& employee);
	bool remove(Employee& employee);
	//Employee*& operator[](int index);
	int getCounter() { return counter; }
};

#endif //HASH_TABLE_H

