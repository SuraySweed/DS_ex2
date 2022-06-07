#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <iostream>
#include "employee.h"
#include "linked_list.h"

typedef enum {
	HASH_TABLE_SUCCESS = 0,
	HASH_TABLE_EMPLOYEE_NOT_FOUND = -1,
	HASH_TABLE_ALLOCATION_ERROR = -2,
	HASH_TABLE_EMPLOYEE_ALREADY_EXISTS = -4,
} HashTableStatus;

class HashTable {
private:
	int size;
	int counter;
	LinkedList** employees; //pointer to array of LinkedList, the value of every index of the array is a linked list of employees 
	static const int init_size = 256;

public:
	HashTable();
	HashTable (const HashTable& hash_table);
	HashTable& operator=(const HashTable& node) = default;
	~HashTable();

	void resize(int new_size, LinkedList* old_employees_array[], int old_size);
	int HashFunction(int employee_id);
	Employee* find(int employee_id);

	HashTableStatus insert(Employee& employee);
	HashTableStatus remove(Employee& employee);
	//Employee*& operator[](int index);
	int getCounter() { return counter; }
	void updateEmployeesCompanyID(int acquirerID);
	void mergeTwoHashies(HashTable acquirer, HashTable target);
	void deleteEmployees();
};

#endif //HASH_TABLE_H

