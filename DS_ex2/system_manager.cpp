#include "system_manager.h"

SystemManager::SystemManager(int k) : number_of_companies(k), number_of_employees(0), 
	companies(new InvertedTree<Company*> * [k]), employeesTable(new HashTable()), 
	employeesTree(new RankedAVL<Employee>())

{
	/*
	*  initialize the array of the companies, 
	* the key of the array is the index of the company thats mean the companyID
	* and its value is a pointer to company object- contain all the data of the company
	*/
	for (int i = 0; i < number_of_companies; i++) {
		companies[i] = new InvertedTree<Company*>(i + 1, new Company(i + 1, i + 1));
	}
}

SystemManager::~SystemManager()
{
	for (int i = 0; i < number_of_companies; i++) {
		delete companies[i];
	}
	delete[] companies;
	delete employeesTable;
	delete employeesTree;
}

StatusType SystemManager::AddEmployee(int employeeID, int companyID, int grade)
{
	if (employeeID <= 0 || companyID <= 0 || companyID > number_of_companies || grade < 0) {
		return INVALID_INPUT;
	}

	if (employeesTable->find(employeeID)) {
		return FAILURE;
	}

	std::shared_ptr<int> company_id = std::make_shared<int>();
	*company_id = companyID;
	Employee employee(employeeID, 0, grade, company_id);
	
	/*
	// adding employee to the company in the companies array
	// getData return the company
	if (!companies[companyID - 1]->find(companies[companyID - 1])->getData()->addEmployee(&employee)) {
		company_id.reset();
		return FAILURE;
	}
	*/

	if (employeesTable->insert(employee) != HASH_TABLE_SUCCESS) {
		company_id.reset();
		return FAILURE;
	}
	number_of_employees++;

	return SUCCESS;
}

StatusType SystemManager::RemoveEmployee(int employeeID)
{
	if (employeeID <= 0) {
		return INVALID_INPUT;
	}

	Employee* employee = employeesTable->find(employeeID);
	if (!employee) {
		return FAILURE;
	}
	int salary = employee->getSalary();
	int company_id = employee->getCompanyID();
	

	// remove employee from the hash table
	if (employeesTable->remove(*employee) != HASH_TABLE_SUCCESS) {
		return FAILURE;
	}

	if (salary > 0) {
		TreeNode<Employee>* returnedNode = employeesTree->remove(employee); // return nullptr if there is one node, and we remove it3
		int company_employees_number = employeesTree->getNumberOfNodes();

		// remove from the big tree
		if (!(!returnedNode && company_employees_number == 1) || !(returnedNode && company_employees_number > 1)) {
			return FAILURE;
		}

		// remove the employee from the company
		if (!companies[company_id - 1]->find(companies[company_id - 1])->getData()->removeEmployee(employee)) {
			return FAILURE;
		}
	}
	number_of_employees--;
	return SUCCESS;
}

StatusType SystemManager::AcquireCompany(int companyID1, int companyID2, double factor)
{
	return StatusType();
}

StatusType SystemManager::EmployeeSalaryIncrease(int employeeID, int salaryIncrease)
{
	return StatusType();
}
