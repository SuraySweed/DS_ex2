#include "system_manager.h"
#include <cmath>

bool SystemManager::removeEmployeeeFromEmployeesTree(Employee* employee)
{
	// return nullptr if there is one node, and we remove it
	TreeNode<Employee>* returnedNode = employeesTree->remove(employee, employee->getGrade()); 
	int company_employees_number = employeesTree->getNumberOfNodes();
	if ((!returnedNode && company_employees_number == 0) || (returnedNode && company_employees_number >= 1)) {
		return true;
	}

	return false;
}

bool SystemManager::removeEmployeeeFromTheCompany(Employee* employee, int company_id)
{
	if (!getCompany(company_id)->removeEmployee(employee)) {
		return false;
	}

	return true;
}

void SystemManager::decZeroSalaryDataInCompany(Employee* employee, int company_id)
{
	getCompany(company_id)->decZeroSalaryEmployees(employee);
}

bool SystemManager::insertEmployeeToTreeAndCompany(Employee* employee, int company_id)
{
	if (!employeesTree->insert(employee, employee->getGrade()) ||
		!getCompany(company_id)->addEmployee(employee))
	{
		return false;
	}

	return true;
}

Company* SystemManager::getCompany(int company_id)
{
	return (companies[company_id - 1]->getData());
}

InvertedTree* SystemManager::getInvertedTreeCompany(int company_id)
{
	return (companies[company_id - 1]->find(companies[company_id - 1]));
}

void SystemManager::updateCompanyIDForEmployees(int targetID, int acquirerID)
{
	Company* target_company = getCompany(targetID);
	target_company->updateEmployeesCompanyID(acquirerID);
}

int SystemManager::getNumberOfZeroSalaryEmployees()
{
	return (number_of_employees - employeesTree->getNumberOfNodes());
}


SystemManager::SystemManager(int k) : number_of_companies(k), number_of_employees(0), 
	sumOfGradesZeroSalary(0), companies(new InvertedTree * [k]), employeesTable(new HashTable()), 
	employeesTree(new RankedAVL<Employee>())
{
	/*
	*  initialize the array of the companies, 
	* the key of the array is the index of the company thats mean the companyID
	* and its value is a pointer to company object- contain all the data of the company
	*/
	for (int i = 0; i < number_of_companies; i++) {
		companies[i] = new InvertedTree(i + 1, new Company(i + 1, i + 1));
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
	
	Company* myCompany = companies[companyID - 1]->find(companies[companyID - 1])->getData();
	std::shared_ptr<int> company_id = std::make_shared<int>();
	*company_id = myCompany->getCompanyID();
	Employee employee(employeeID, 0, grade, company_id);

	if ((employeesTable->insert(employee) != HASH_TABLE_SUCCESS) ||
		!(myCompany->addEmployeeToCompanyHashTable(&employee))) {
		company_id.reset();
		return FAILURE;
	}

	myCompany->incZeroSalaryEmployees(&employee);
	sumOfGradesZeroSalary += grade;
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
	int grade = employee->getGrade();
	
	Employee employeeCopy(employeeID, salary, grade, employee->getCompanyIDPtr());

	if ((employeesTable->remove(*employee) != HASH_TABLE_SUCCESS) || 
		!getCompany(company_id)->removeEmployeeFromEmployeesHash(&employeeCopy)){
		return FAILURE;
	}

	if (salary > 0) {
		if (!removeEmployeeeFromEmployeesTree(&employeeCopy) || 
			!removeEmployeeeFromTheCompany(&employeeCopy, company_id)) {
			return FAILURE;
		}
	}
	else {
		sumOfGradesZeroSalary -= grade;
		decZeroSalaryDataInCompany(&employeeCopy, company_id);
	}
 
	number_of_employees--;
	return SUCCESS;
}

StatusType SystemManager::AcquireCompany(int acquirerID, int targetID, double factor)
{
	if (acquirerID <= 0 || targetID <= 0 || acquirerID > number_of_companies || targetID > number_of_companies ||
		acquirerID == targetID || factor <= 0) {
		return INVALID_INPUT;
	}

	InvertedTree* acquirerCompany = getInvertedTreeCompany(acquirerID);
	InvertedTree* targetCompany = getInvertedTreeCompany(targetID);

	// acquirerCompany already bought the targetCompany
	if (acquirerCompany == targetCompany) return INVALID_INPUT; 
	
	InvertedTree* owner = companies[companies[targetID - 1]->find(companies[targetID - 1])->getData()->getOwnerID() - 1];
	companies[targetID - 1]->Union(acquirerCompany, targetCompany, factor, owner);

	return SUCCESS;

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
	int grade = old_employee->getGrade();
	Employee new_employee(employeeID, new_salary, old_employee->getGrade(), old_employee->getCompanyIDPtr());

	// update the salary in the employees hash table, and in the employees company hash table
	employeesTable->find(employeeID)->setSalary(new_salary);			    // O(1)
	
	getCompany(company_id)->updateSalaryToEmployee(employeeID, new_salary); // O(1)

	Employee EmployeeToDelete(employeeID, old_salary, grade, old_employee->getCompanyIDPtr());
	// update the trees
	if (old_salary > 0) {
		if (!removeEmployeeeFromEmployeesTree(&EmployeeToDelete) ||
			!removeEmployeeeFromTheCompany(&EmployeeToDelete, company_id)) {
			return FAILURE;
		}

		if (!insertEmployeeToTreeAndCompany(&new_employee, company_id)) {
			
			return FAILURE;
		}
	}

	// old salary was 0
	else {
		sumOfGradesZeroSalary -= grade;
		decZeroSalaryDataInCompany(&new_employee, company_id);
		if (!insertEmployeeToTreeAndCompany(&new_employee, company_id)) {
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

	// employee doesn't exist 
	Employee* old_employee = employeesTable->find(employeeID);
	if (!old_employee) {
		return FAILURE;
	}

	int old_grade = old_employee->getGrade();
	int salary = old_employee->getSalary();
	int new_grade = old_grade + bumpGrade;
	int company_id = old_employee->getCompanyID();
	Employee new_employee(employeeID, salary, new_grade, old_employee->getCompanyIDPtr());
	
	if (bumpGrade > 0) {
		// update the employee grade in the employees hash table, and company employees hash table
		employeesTable->find(employeeID)->setGrade(new_grade);
		getCompany(company_id)->updateGradeForEmployee(employeeID, new_grade);

		Employee EmployeeToDelete(employeeID, salary, old_grade, old_employee->getCompanyIDPtr());
		// employee exist in the cmployees company tree, employees hash table and in the employees salary tree 
		if (salary > 0) {
			Employee oldEmployee(employeeID, salary, old_grade, old_employee->getCompanyIDPtr());
			if (!removeEmployeeeFromEmployeesTree(&oldEmployee) ||
				!removeEmployeeeFromTheCompany(&oldEmployee, company_id)) {
				return FAILURE;
			}

			if (!insertEmployeeToTreeAndCompany(&new_employee, company_id)) {
				return FAILURE;
			}
		}
		else {
			getCompany(company_id)->addToSumGradesZeroSalary(new_grade - old_grade);
			sumOfGradesZeroSalary += (new_grade - old_grade);
		}
	}

	return SUCCESS;
}

StatusType SystemManager::SumOfBumpGradeBetweenTopWorkersByGroup(int companyID, int m)
{
	if (companyID > number_of_companies || companyID < 0 || m <= 0) {
		return INVALID_INPUT;
	}

	long long* sumBumpGrade = new long long();
	if (companyID > 0) {
		Company* company = companies[companyID - 1]->find(companies[companyID - 1])->getData();
		if (company->getNumberOfEmployeesNonZero() < m) {
			delete sumBumpGrade;
			return FAILURE;
		}
		company->sumBumpGradesInCompany(m, (long long*)sumBumpGrade);
	}

	else {
		if (employeesTree->getNumberOfNodes() < m) {
			delete sumBumpGrade;
			return FAILURE;
		}
		employeesTree->sumBumpGrade(m, (long long*)sumBumpGrade);
	}

	printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", *sumBumpGrade);
	delete sumBumpGrade;
	return SUCCESS;
}

StatusType SystemManager::AverageBumpGradeBetweenSalaryByGroup(int companyID, 
	int lowerSalary, int higherSalary)
{
	if (higherSalary < 0 || lowerSalary < 0 || lowerSalary > higherSalary ||
		companyID < 0 || companyID > number_of_companies) {
		return INVALID_INPUT;
	}

	int lowSalary = lowerSalary;
	int highRank = 0;
	long long highSum = 0;
	int lowRank = 0;
	long long lowSum = 0; 

	//get average of employees in specific company
	if (companyID > 0) {
		Company* company = companies[companyID - 1]->find(companies[companyID - 1])->getData();
		if (company->getNumberOfEmployees() == 0) {
			return FAILURE;
		}

		RankedAVL<Employee>* employees_tree = company->getEmployeesTree();
		TreeNode<Employee>* base_root = employees_tree->getRoot();

		if ((employees_tree->getMaxNodeData(base_root) && (lowerSalary > employees_tree->getMaxNodeData(base_root)->getSalary())) ||
			((higherSalary != 0 && lowerSalary != 0) && (employees_tree->getMinNodeData(base_root) &&
			(higherSalary < employees_tree->getMinNodeData(base_root)->getSalary()))) ||
			(lowerSalary == 0 && (higherSalary == 0 || (employeesTree->getMinNodeData(base_root) && 
			(higherSalary < employeesTree->getMinNodeData(base_root)->getSalary()))) && company->getNumOfZeroSalaryEmployees() == 0)) {
			return FAILURE;
		}
		
		if (higherSalary != 0) {
			if (lowerSalary == 0 && employees_tree->getMinNodeData(base_root)) {
				lowSalary = employees_tree->getMinNodeData(base_root)->getSalary();
			}
			TreeNode<Employee>* highNode = nullptr, *lowNode = nullptr;
			highNode = employees_tree->getLastInInterval(base_root, higherSalary);
			if (highNode) {
				lowNode = employees_tree->getFirstInInterval(base_root, lowSalary);
			} 
			if (highNode && lowNode) {
				employees_tree->calcRank(base_root, highNode, &highRank);
				employees_tree->calcRank(base_root, lowNode, &lowRank);
				lowRank += 1;
				employees_tree->calcSumOfGrades(base_root, highNode, &highSum);
				employees_tree->calcSumOfGrades(base_root, lowNode, &lowSum);
				lowSum += lowNode->Grade;
			}
		}
		if (lowerSalary == 0) {
			lowSum += company->getSumOfGradesOfZeroSalaryEmployees();
			lowRank += company->getNumOfZeroSalaryEmployees();
		}
	}
	//get average of all Employees
	else {

		TreeNode<Employee>* base_root = employeesTree->getRoot();

		if ((employeesTree->getMaxNodeData(base_root) && (lowerSalary > employeesTree->getMaxNodeData(base_root)->getSalary())) ||
			((higherSalary != 0 && lowerSalary != 0) && (employeesTree->getMinNodeData(base_root)) && 
			(higherSalary < employeesTree->getMinNodeData(base_root)->getSalary())) ||
			(lowerSalary == 0 && (higherSalary == 0 || (employeesTree->getMinNodeData(base_root) && 
			(higherSalary < employeesTree->getMinNodeData(base_root)->getSalary()))) && getNumberOfZeroSalaryEmployees() == 0)) {
			return FAILURE;
		}

		if (higherSalary != 0) {
			if (lowerSalary == 0 && employeesTree->getMinNodeData(base_root)) {
				lowSalary = employeesTree->getMinNodeData(base_root)->getSalary();
			}
			TreeNode<Employee>* highNode = nullptr, * lowNode = nullptr;
			highNode = employeesTree->getLastInInterval(base_root, higherSalary);
			if (highNode) {
				lowNode = employeesTree->getFirstInInterval(base_root, lowSalary);
			}

			if (highNode && lowNode) {
				employeesTree->calcRank(base_root, highNode, &highRank);
				employeesTree->calcRank(base_root, lowNode, &lowRank);
				lowRank += 1;
				employeesTree->calcSumOfGrades(base_root, highNode, &highSum);
				employeesTree->calcSumOfGrades(base_root, lowNode, &lowSum);
				lowSum += lowNode->Grade;
			}
		}
		if (lowerSalary == 0) {
			lowSum += getSumOfGradeZeroSalary();
			lowRank += getNumberOfZeroSalaryEmployees();
		}
	}

	
	/*long double averageBumpGrade = 0;
	averageBumpGrade = (long double)((long double)(lowSum - highSum) / (long double)(lowRank - highRank));

	unsigned int temp = (unsigned int)(((long double)averageBumpGrade) * 10);
	long double average = ((long double)temp) / 10;
	if (abs(average + 0.1 - ((long double)averageBumpGrade)) <= 0.0000000001) {
		average += 0.1;
	}*/

	long double average = (long double)((long double)(lowSum - highSum) / (long double)(lowRank - highRank));	
	printf("AverageBumpGradeBetweenSalaryByGroup: %.1Lf\n", average);
	
	//printf("AverageBumpGradeBetweenSalaryByGroup: %.1Lf\n", floor(10 * average + 0.5f) / 10);

	return SUCCESS;
}

StatusType SystemManager::CompanyValue(int companyID)
{
	if (companyID > number_of_companies || companyID <= 0) {
		return INVALID_INPUT;
	}

	InvertedTree* company_node = companies[companyID - 1];
	InvertedTree* head = companies[companyID - 1]->find(company_node);

	long double result;
	result = (long double)(company_node->getData()->getValue());
	result += (company_node->getAcquiredValue() + (head != company_node ? head->getAcquiredValue() : 0));
	printf("CompanyValue: %.1Lf\n", result);
	return SUCCESS;
}
