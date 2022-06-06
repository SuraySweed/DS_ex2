#include "system_manager.h"

bool SystemManager::removeEmployeeeFromEmployeesTree(Employee* employee)
{
	// return nullptr if there is one node, and we remove it
	TreeNode<Employee>* returnedNode = employeesTree->remove(employee, employee->getGrade()); 
	int company_employees_number = employeesTree->getNumberOfNodes();
	if (!(!returnedNode && company_employees_number == 0) || !(returnedNode && company_employees_number >= 1)) {
		return false;
	}

	return true;
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
	return (companies[company_id - 1]->find(companies[company_id - 1])->getData());
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

/*void SystemManager::fillGradesArray(Employee** employees_arr, int grades[] , int size)
{
	for (int i = 0; i < size; i++) {
		grades[i] = employees_arr[i]->getGrade();
	}
}

void SystemManager::mergeCompaniesTrees(Company* acquirerCompany, Company* targetCompany)
{
	int target_employees_number = targetCompany->getNumOfEmployeesInTree();
	int* target_grades = new int[target_employees_number];
	Employee** target_employees_arr = new Employee * [target_employees_number];
	int acquirer_employees_number = acquirerCompany->getNumOfEmployeesInTree();
	int* acquirer_grades = new int[acquirer_employees_number];
	Employee** acquirer_employees_arr = new Employee * [acquirer_employees_number];

	targetCompany->fillEmployeesInArray(target_employees_arr);
	fillGradesArray(target_employees_arr, target_grades, target_employees_number);
	acquirerCompany->fillEmployeesInArray(acquirer_employees_arr);
	fillGradesArray(acquirer_employees_arr, acquirer_grades, acquirer_employees_number);

	int total_employees = target_employees_number + acquirer_employees_number;
	int* total_grades = new int[total_employees];
	Employee** total_employees_arr = new Employee * [total_employees];

	Employee** target_fill_array = new Employee * [target_employees_number];
	Employee** acquire_fill_array = new Employee * [acquirer_employees_number];
	
	for (int i = 0; i < target_employees_number; i++) {
		target_fill_array[i] = new Employee(*(target_employees_arr[i]));
	}

	for (int i = 0; i < acquirer_employees_number; i++) {
		acquire_fill_array[i] = new Employee(*(acquirer_employees_arr[i]));
	}

	acquirerCompany->getEmployeesTree()->mergeTree(target_fill_array, target_grades, target_employees_number,
		acquire_fill_array, acquirer_grades, acquirer_employees_number, total_employees_arr, total_grades);

	delete targetCompany->getEmployeesTree();

	for (int i = 0; i < target_employees_number; i++) {
		delete target_fill_array[i];
	}

	for (int i = 0; i < acquirer_employees_number; i++) {
		delete acquire_fill_array[i];
	}

	delete[] target_fill_array;
	delete[] acquire_fill_array;
}

void SystemManager::mergeCompaniesHashies(Company* acquirerCompany, Company* targetCompany)
{
	acquirerCompany->getEmployeesHashTable().mergeTwoHashies(acquirerCompany->getEmployeesHashTable(), targetCompany->getEmployeesHashTable());
}*/

/*
void SystemManager::updateCompanyIDForEmployeesByInorder(TreeNode<Employee>* root, int acquirerID
	, int targetEmployeesNumber, int i)
{
	if (root == nullptr || targetEmployeesNumber <= 0)
		return;
	updateCompanyIDForEmployeesByInorder(root->left, acquirerID, targetEmployeesNumber, i);
	if (i < targetEmployeesNumber) {
		root->data->setEmployerID(acquirerID);
		i++;
		updateCompanyIDForEmployeesByInorder(root->right, acquirerID, targetEmployeesNumber, i);
	}
	return;
}
*/

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

	std::shared_ptr<int> company_id = std::make_shared<int>();
	*company_id = companyID;
	Employee employee(employeeID, 0, grade, company_id);
	
	if ((employeesTable->insert(employee) != HASH_TABLE_SUCCESS) ||
		!(getCompany(companyID)->addEmployeeToCompanyHashTable(&employee))) {
		company_id.reset();
		return FAILURE;
	}

	getCompany(companyID)->incZeroSalaryEmployees(&employee);
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
	
	// remove employee from the hash table and from the employees company hash table
	if ((employeesTable->remove(*employee) != HASH_TABLE_SUCCESS) && 
		!getCompany(company_id)->removeEmployeeFromEmployeesHash(employee)){
		return FAILURE;
	}

	if (salary > 0) {
		if (!removeEmployeeeFromEmployeesTree(employee) && 
			!removeEmployeeeFromTheCompany(employee, company_id)) {
			return FAILURE;
		}
	}
	else {
		sumOfGradesZeroSalary -= grade;
		decZeroSalaryDataInCompany(employee, company_id);
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
	if (acquirerCompany == targetCompany) return SUCCESS; ///// check if we have to return INVALID_INPUT
	
	// update target company id to acquirer company id in the 3 data structures: company tree and hash table, hash and the big tree
	updateCompanyIDForEmployees(targetID, acquirerID);

	//union the two groups with updating the values
	companies[targetID - 1]->Union(acquirerCompany, targetCompany, factor);
	//mergeCompaniesTrees(acquirerCompany, targetCompany);
	//mergeCompaniesHashies(acquirerCompany, targetCompany);

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

	// update the trees
	if (old_salary > 0) {
		if (!removeEmployeeeFromEmployeesTree(old_employee) &&
			!removeEmployeeeFromTheCompany(old_employee, company_id)) {
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
	Employee new_employee(employeeID, salary, old_employee->getGrade(), old_employee->getCompanyIDPtr());
	
	if (bumpGrade > 0) {
		// update the employee grade in the employees hash table, and company employees hash table
		employeesTable->find(employeeID)->setGrade(new_grade);
		getCompany(company_id)->updateGradeForEmployee(employeeID, new_grade);

		// employee exist in the cmployees company tree, employees hash table and in the employees salary tree 
		if (salary > 0) {
			if (!removeEmployeeeFromEmployeesTree(old_employee) &&
				!removeEmployeeeFromTheCompany(old_employee, company_id)) {
				return FAILURE;
			}

			if (!insertEmployeeToTreeAndCompany(&new_employee, company_id)) {
				return FAILURE;
			}
		}
		else {
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

	int* sumBumpGrade = new int();
	if (companyID > 0) {
		if (getCompany(companyID)->getNumberOfEmployeesNonZero() < m) {
			delete sumBumpGrade;
			return FAILURE;
		}
		getCompany(companyID)->sumBumpGradesInCompany(m, (int*)sumBumpGrade);
	}

	else {
		if (employeesTree->getNumberOfNodes() < m) {
			delete sumBumpGrade;
			return FAILURE;
		}
		employeesTree->sumBumpGrade(m, (int*)sumBumpGrade);
	}

	printf("SumOfBumpGradeBetweenTopWorkersByGroup: %d\n", *sumBumpGrade);
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
	int highSum = 0;
	int lowRank = 0;
	int lowSum = 0; 

	//get average of employees in specific company
	if (companyID > 0) {
		Company* company = getCompany(companyID);
		RankedAVL<Employee>* employees_tree = company->getEmployeesTree();
		TreeNode<Employee>* base_root = employees_tree->getRoot();

		if (lowerSalary > employees_tree->getMaxNodeData(base_root)->getSalary() || 
			((higherSalary != 0) && higherSalary < employees_tree->getMinNodeData(base_root)->getSalary()) ||
			(higherSalary == 0 && company->getNumOfZeroSalaryEmployees() == 0)) {
			return FAILURE;
		}
		
		if (higherSalary != 0) {
			if (lowerSalary == 0) {
				lowSalary = employees_tree->getMinNodeData(base_root)->getSalary();
			}
			TreeNode<Employee>* highNode = employees_tree->getLastInInterval(base_root, higherSalary);
			TreeNode<Employee>* lowNode = employees_tree->getFirstInInterval(base_root, lowSalary);

			employees_tree->calcRank(base_root, highNode, &highRank);
			employees_tree->calcRank(base_root, lowNode, &lowRank);
			lowRank += 1;
			employees_tree->calcSumOfGrades(base_root, highNode, &highSum);
			employees_tree->calcSumOfGrades(base_root, lowNode, &lowSum);
			lowSum += lowNode->Grade;

		}
		if (lowerSalary == 0) {
			lowSum += company->getSumOfGradesOfZeroSalaryEmployees();
			lowRank += company->getNumOfZeroSalaryEmployees();
		}
	}
	//get average of all Employees
	else {

		TreeNode<Employee>* base_root = employeesTree->getRoot();

		if (lowerSalary > employeesTree->getMaxNodeData(base_root)->getSalary() ||
			((higherSalary != 0) && higherSalary < employeesTree->getMinNodeData(base_root)->getSalary()) ||
			(higherSalary == 0 && getNumberOfZeroSalaryEmployees() == 0)) {
			return FAILURE;
		}

		if (higherSalary != 0) {
			if (lowerSalary == 0) {
				lowSalary = employeesTree->getMinNodeData(base_root)->getSalary();
			}
			
			TreeNode<Employee>* highNode = employeesTree->getLastInInterval(base_root, higherSalary);
			TreeNode<Employee>* lowNode = employeesTree->getFirstInInterval(base_root, lowSalary);

			employeesTree->calcRank(base_root, highNode, &highRank);
			employeesTree->calcRank(base_root, lowNode, &lowRank);
			employeesTree->calcSumOfGrades(base_root, highNode, &highSum);
			employeesTree->calcSumOfGrades(base_root, lowNode, &lowSum);
		}
		if (lowerSalary == 0) {
			lowSum += getSumOfGradeZeroSalary();
			lowRank += getNumberOfZeroSalaryEmployees();
		}
	}
	double average = 0;
	// !!!!! devide by 0 !!!!! check this state
	average = ((lowSum - highSum) / (lowRank - highRank));
	printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", average);
	return SUCCESS;
}

StatusType SystemManager::CompanyValue(int companyID)
{
	if (companyID > number_of_companies || companyID <= 0) {
		return INVALID_INPUT;
	}

	InvertedTree* company_node = companies[companyID - 1];
	InvertedTree* head = companies[companyID - 1]->find(company_node);

	double result;
	result = (double)(company_node->getData()->getValue());
	result += company_node->getAcquiredValue() + head->getAcquiredValue();
	printf("CompanyValue: %.1f\n", result);
	return SUCCESS;
}
