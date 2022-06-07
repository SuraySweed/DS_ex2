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
		for (int listIndex = 0; listIndex < (hash_table.employees)[i]->getSize() && currentNode; listIndex++) {
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

// too dooo
void HashTable::resize(int new_size, LinkedList* old_employees_array[], int old_size)
{
	size = new_size;
	LinkedList** new_employees_arr = new LinkedList * [new_size];
	for (int i = 0; i < size; i++)
	{
		new_employees_arr[i] = new LinkedList();
	}

	//LinkedList* employeesInBlock = new LinkedList();

	int count = 0;
	for (int i = 0; i < old_size; i++) {
		if (old_employees_array[i]) {
			Node* currentNode = old_employees_array[i]->getHead();
			while (currentNode) {
				//employeesInBlock->insert(*(currentNode->data));
				new_employees_arr[i]->insert(*(currentNode->data));
				count++;
				currentNode->data = nullptr;
				currentNode = currentNode->next;
			}
			delete old_employees_array[i];
		}
	}
	delete[] old_employees_array;

	/*
	for (int i = 0; i < size; i++)
	{
		new_employees_arr[i] = nullptr;
	}
	*/
	/*
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
	*/
	//delete employeesInBlock;
	employees = new_employees_arr;

}

int HashTable::HashFunction(int employee_id)
{
	return (employee_id % size);
}

Employee* HashTable::find(int employee_id)
{
	int index = HashFunction(employee_id);
	Node* employee = employees[index]->find(employee_id);
	if (employees[index]->find(employee_id)) {
		//Employee* sa = employees[index]->find(employee_id)->data;
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

	if (!employees[index]) {
		employees[index] = new LinkedList();
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
		employees[index] = nullptr;
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
	for (int i = 0; i < target_size; i++) {
		Node* currentNode = (target->employees)[i]->getHead();
		//for (int listIndex = 0; /*listIndex < (target.employees)[i]->getSize() && */currentNode; listIndex++) {
		while(currentNode) {
			acquirer->insert(*(currentNode->data));
			//target.remove(*(currentNode->data)); // check that
			//currentNode = currentNode->next;
			Node* temp = currentNode->next;
			target->remove(*(currentNode->data));
			currentNode = temp;
		}
	}
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

/*
Employee*& HashTable::operator[](int index)
{
	if (index < 0 || index >= size) {
		//throw exception
	}
	//return employees[index];
}
*/