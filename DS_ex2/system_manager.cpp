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
	companies[companyID - 1]->find(companies[companyID - 1])->getData()->incZeroSalaryEmployees(&employee);

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
		// remove from the big tree
		TreeNode<Employee>* returnedNode = employeesTree->remove(employee); // return nullptr if there is one node, and we remove it3
		int company_employees_number = employeesTree->getNumberOfNodes();
		if (!(!returnedNode && company_employees_number == 1) || !(returnedNode && company_employees_number > 1)) {
			return FAILURE;
		}

		// remove the employee from the company
		if (!companies[company_id - 1]->find(companies[company_id - 1])->getData()->removeEmployee(employee)) {
			return FAILURE;
		}
	}
	else {
		companies[company_id - 1]->find(companies[company_id - 1])->getData()->decZeroSalaryEmployees(employee);
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
	if (employeeID <= 0 || salaryIncrease <= 0) {
		return INVALID_INPUT;
	}

	Employee* old_employee = employeesTable->find(employeeID);
	if (!old_employee) {
		return FAILURE;
	}

	// remove the employee from the 2 trees, and insert the employee with the new salary
	// we update the salary in the hash table in O(1) complexity time
	int old_salary = old_employee->getSalary();
	int new_salary = old_salary + salaryIncrease;
	int company_id = old_employee->getCompanyID();
	//Employee new_employee(employeeID, new_salary, old_employee->getGrade(), old_employee->getCompanyIDPtr());
	
	std::shared_ptr<int> company_id_ptr = std::make_shared<int>();
	*company_id_ptr = company_id;
	Employee new_employee(employeeID, new_salary, old_employee->getGrade(), company_id_ptr);
	// update the salary in the employees hash table
	employeesTable->find(employeeID)->setSalary(new_salary);

	// update the trees
	if (old_salary > 0) {
		TreeNode<Employee>* returnedNode = employeesTree->remove(old_employee);
		int company_employees_number = employeesTree->getNumberOfNodes();
		if (!(!returnedNode && company_employees_number == 1) || !(returnedNode && company_employees_number > 1)) {
			return FAILURE;
		}

		// remove the employee from the company
		if (!companies[company_id - 1]->find(companies[company_id - 1])->getData()->removeEmployee(old_employee)) {
			return FAILURE;
		}

		if (!employeesTree->insert(&new_employee) && 
			!companies[company_id - 1]->find(companies[company_id - 1])->getData()->addEmployee(&new_employee))
		{
			return FAILURE;
		}
	}
	// old salary was 0
	else {
		companies[company_id - 1]->find(companies[company_id - 1])->getData()->decZeroSalaryEmployees(&new_employee);
		companies[company_id - 1]->find(companies[company_id - 1])->getData()->addEmployee(&new_employee);
		if (!employeesTree->insert(&new_employee)) {
			return FAILURE;
		}
	}

	return SUCCESS;
}

StatusType SystemManager::PromoteEmployee(int employeeID, int bumpGrade)
{
	if (employeeID <= 0) {
		return INVALID_INPUT;
	} 
}
