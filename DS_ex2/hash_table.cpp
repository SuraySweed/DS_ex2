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

	/*
	for (int i = 0; i < old_size; i++) {
		if (old_employees_array[i]) {
			delete old_employees_array[i];
		}
	}
	
	delete[] old_employees_array;*/
	
	employees = new_employees_arr;
}

int HashTable::HashFunction(int employee_id)
{
	return (employee_id % size);
}

Employee* HashTable::find(int employee_id)
{
	int index = HashFunction(employee_id);
	//Node* employee = employees[index]->find(employee_id);
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
		//delete employees[index]; //delete linkedlist
		//employees[index] = nullptr;
		//employees[index] = new LinkedList();
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
			//target->remove(*(currentNode->data)); // check that
			currentNode = currentNode->next;
			
			/*
			Node* temp = currentNode->next;
			//target->remove(*(currentNode->data));
			currentNode = temp;
			*/
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

void HashTable::printEmployees(int company, int i)
{
	printf("employees of company %d:\n", company);
	for (int k = 0; k < size; k++) {
		LinkedList* l = employees[k];
		Node* n = l->getHead();

		while (n) {
			printf("EmployeeID: %d, Salary: %d, Grade: %d \n", n->data->getID(), n->data->getSalary(), n->data->getGrade());
			n = n->next;
		}
	}
	printf("\n");
}

void HashTable::printE()
{
	for (int i = 0; i < size; i++)
	{
		LinkedList* l = employees[i];
		Node* n = l->getHead();
		while (n) {
			printf("Employee ID %d, company %d\n", n->data->getID(), n->data->getCompanyID());
			n = n->next;
		}
	}
}

/*
LinkedList* HashTable::operator[](int index)
{
	
	if (index < 0 || index >= size) {
		//throw exception
	}
	
	return employees[index];
}
*/