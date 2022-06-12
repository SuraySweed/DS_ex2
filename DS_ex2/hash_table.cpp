#include "hash_table.h"

HashTable::HashTable() : size(init_size), counter(0)
{
	employees = new LinkedList * [size];
	for (int i = 0; i < size; i++) {
		employees[i] = new LinkedList();
	}
}

HashTable::HashTable(const HashTable& hash_table) : size(hash_table.size), counter(hash_table.counter)
{
	this->employees = new LinkedList * [size];
	for (int i = 0; i < size; i++) {
		this->employees[i] = new LinkedList();
	}
	for (int i = 0; i < size; i++) {
		Node* currentNode = (hash_table.employees)[i]->getHead();
		while (currentNode) {
			this->insert(*(currentNode->data));
			currentNode = currentNode->next;
		}
	}
}

HashTable::~HashTable()
{
	for (int i = 0; i < size; i++) {
		if (employees[i]) {
			delete employees[i];
			size--;
		}
	}
	delete[] employees;
}

void HashTable::resize(int new_size, LinkedList* old_employees_array[], int old_size)
{
	/*
	size = new_size;
	LinkedList** new_employees_arr = new LinkedList * [new_size];
	for (int i = 0; i < size; i++)
	{
		new_employees_arr[i] = new LinkedList();
	}
	int index = 0;

	for (int i = 0; i < old_size; i++) {
		if (old_employees_array[i]) {
			Node* currentNode = old_employees_array[i]->getHead();
			while (currentNode) {
				index = HashFunction(currentNode->data->getID());
				new_employees_arr[index]->insert(*(currentNode->data));
				currentNode = currentNode->next;
			}
		}
	}
	employees = new_employees_arr;*/


	size = new_size;
	LinkedList* all_employees = new LinkedList();
	int counter = 0;

	for (int i = 0; i < old_size; i++)
	{
		if (old_employees_array[i]) {
			Node* currentNode = old_employees_array[i]->getHead();
			while (currentNode)
			{
				all_employees->insert(*(currentNode->data));
				counter++;
				currentNode->data = nullptr;
				currentNode = currentNode->next;
			}
			delete old_employees_array[i];
		}
	}

	delete[] old_employees_array;

	LinkedList** new_employees_arr = new LinkedList * [size];
	for (int i = 0; i < size; i++)
	{
		new_employees_arr[i] = new LinkedList();
	}

	Node* currentNode = all_employees->getHead();
	for (int i = 0; i < counter; i++)
	{
		Employee* currentEmployee = currentNode->data;
		currentNode->data = nullptr;
		int index = HashFunction(currentEmployee->getID());
		if (!new_employees_arr[index])
		{
			new_employees_arr[index] = new LinkedList();
		}
		new_employees_arr[index]->insert(*currentEmployee);
		currentNode = currentNode->next;
	}
	delete all_employees;
	employees = new_employees_arr;
}

int HashTable::HashFunction(int employee_id)
{
	return (employee_id % size);
}

Employee* HashTable::find(int employee_id)
{
	int index = HashFunction(employee_id);
	if (employees[index]->find(employee_id)) {
		return ((employees[index]->find(employee_id))->data); // find in linkedlist
	}
	return nullptr;
}

HashTableStatus HashTable::insert(Employee& employee)
{
	if (counter == size / 2) {
		resize(2 * size, employees, size);
	}
	
	int index = HashFunction(employee.getID());
	
	if (employees[index] && find(employee.getID())) {
		return HASH_TABLE_EMPLOYEE_ALREADY_EXISTS;
	}
	employees[index]->insert(employee);
	counter++;
	return HASH_TABLE_SUCCESS;
}

HashTableStatus HashTable::remove(Employee& employee)
{
	if (counter == size / 4) {
		resize((int)(0.5 * size), employees, size);
	}

	int index = HashFunction(employee.getID());
	if (!employees[index] || !employees[index]->find(employee.getID())) {
		return HASH_TABLE_EMPLOYEE_NOT_FOUND;
	}
	
	employees[index]->remove(employee.getID());
	if (!employees[index]->getHead()) {
		delete employees[index]; //delete linkedlist
		employees[index] = new LinkedList();
	}
	counter--;
	return HASH_TABLE_SUCCESS;
}

void HashTable::updateEmployeesCompanyID(int acquirerID)
{
	for (int hashIndex = 0; hashIndex < size; hashIndex++) {
		employees[hashIndex]->updateEmployeesCompanyID(acquirerID);
	}
}

void HashTable::mergeTwoHashies(HashTable* acquirer, HashTable* target)
{
	int target_size = target->getSize();
	LinkedList* target_employees = new LinkedList();

	for (int i = 0; i < target_size; i++) {
		Node* currentNode = (target->employees)[i]->getHead();
		while(currentNode) {
			target_employees->insert(*(currentNode->data));
			currentNode = currentNode->next;
		}
		delete (target->employees[i]);
	}

	delete[] target->employees;

	for (int i = 0; i < target_size; i++) {
		Node* currentNode = target_employees->getHead();
		while(currentNode) {
			acquirer->insert(*(currentNode->data));
			currentNode = currentNode->next;
		}
	}

	delete target_employees;
}

void HashTable::deleteEmployees()
{
	for (int i = 0; i < size; i++) {
		if (employees[i]) {
			delete employees[i];
		}
		size--;
	}
	delete[] employees;
	
}
