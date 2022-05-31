#include "hash_table.h"

HashTable::HashTable() : size(init_size), counter(0)
{
	employees = new LinkedList * [size];
	for (int i = 0; i < size; i++) {
		employees[i] = nullptr;
	}
}

HashTable::~HashTable()
{
	for (int i = 0; i < size; i++) {
		if (employees[i]) {
			delete employees[i];
		}
	}
	delete[] employees;
}

// too dooo
void HashTable::resize(int new_size, LinkedList* old_employees_array[], int old_size)
{
	size = new_size;
	LinkedList** new_employees_arr = new LinkedList * [new_size];
	LinkedList* employeesInBlock = new LinkedList();

	int count = 0;
	for (int i = 0; i < old_size; i++) {
		if (old_employees_array[i]) {
			employeesInBlock = old_employees_array[i];
			count++;
		}
		delete old_employees_array[i];
	}
	delete[] old_employees_array;

	Node* head = employeesInBlock->getHead();
	for (int i = 0; i < count; i++) {
		Employee* currentEmployee = head->data;
		head->data = nullptr;
		int index = HashFunction(currentEmployee->getID());
		if (!new_employees_arr) {
			new_employees_arr[index] = new LinkedList();
		}
		new_employees_arr[index]->insert(*currentEmployee);
		head = head->next;
	}
	delete employeesInBlock;
	employees = new_employees_arr;

}

int HashTable::HashFunction(int employee_id)
{
	return (employee_id % size);
}

Employee* HashTable::find(int employee_id)
{
	int index = HashFunction(employee_id);
	return (employees[index]->find();
}

bool HashTable::insert(Employee& employee)
{
	if (counter == size / 2) {
		resize(2 * size, employees, size);
	}
	
	int index = HashFunction(employee.getID());
	
	if (employees[index] && employees[index]->find(employee)) {
		return false; //employee already exist in the hash table of the employees
	}
	else {
		employees[index] = new LinkedList();
		employees[index]->insert(employee);
		counter++;
	}
	return true;
}

/*
Employee*& HashTable::operator[](int index)
{
	if (index < 0 || index >= size) {
		//throw exception
	}
	//return employees[index];
}
*/