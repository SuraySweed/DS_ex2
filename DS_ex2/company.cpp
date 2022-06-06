#include "company.h"

Company::Company(int companyID, int companyValue) : id(companyID), value(companyValue), 
	num_of_employees_with_zero_salary(0), sum_of_grades_of_zero_salary_employees(0), 
	employees(RankedAVL<Employee>()), all_employees_table(HashTable()) {}

Company::Company(Company* company) : id(company->getCompanyID()), value(company->getValue()),
	num_of_employees_with_zero_salary(company->getNumOfZeroSalaryEmployees()),
	sum_of_grades_of_zero_salary_employees(company->getSumOfGradesOfZeroSalaryEmployees()),
	employees(RankedAVL<Employee>(*(company->getEmployeesTree()))),
	all_employees_table(HashTable(company->getEmployeesHashTable())) {}

/*
Company::Company(const Company& company) : id(company.getCompanyID()), value(company.getValue()),
num_of_employees_with_zero_salary(company.getNumOfZeroSalaryEmployees()),
sum_of_grades_of_zero_salary_employees(company.getSumOfGradesOfZeroSalaryEmployees()),
employees(RankedAVL<Employee>((company.getEmployeesTree())),
	all_employees_table(HashTable(company.getEmployeesHashTable())) {}
*/

bool Company::addEmployee(Employee* employee)
{
	if (employees.insert(employee, employee->getGrade())) {
		return true;
	}
	return false;
}

bool Company::addEmployeeToCompanyHashTable(Employee* employee)
{
	if (all_employees_table.insert(*employee) == HASH_TABLE_SUCCESS) {
		return true;
	}
	return false;
}

bool Company::removeEmployee(Employee* employee)
{
	TreeNode<Employee>* returnedNode = employees.remove(employee, employee->getGrade()); // return nullptr if there is one node, and we remove it3
	int company_employees_number = employees.getNumberOfNodes();
	//HashTableStatus hash_remove_result = all_employees_table.remove(*employee);

	if ((!returnedNode && company_employees_number == 0) || (returnedNode && company_employees_number >= 1)) {
		//hash_remove_result == HASH_TABLE_SUCCESS) {
		return true;
	}
	return false;
}

bool Company::removeEmployeeFromEmployeesHash(Employee* employee)
{
	HashTableStatus hash_remove_result = all_employees_table.remove(*employee);
	if (hash_remove_result == HASH_TABLE_SUCCESS) {
		return true;
	}
	return false;
}

void Company::incZeroSalaryEmployees(Employee* employee)
{
	num_of_employees_with_zero_salary++;
	sum_of_grades_of_zero_salary_employees += employee->getGrade();
}

void Company::decZeroSalaryEmployees(Employee* employee)
{
	if (num_of_employees_with_zero_salary >= 1) {
		num_of_employees_with_zero_salary--;
		sum_of_grades_of_zero_salary_employees -= employee->getGrade();
	}
}

void Company::sumBumpGradesInCompany(int m, int* sum)
{
	employees.sumBumpGrade(m, sum);
}

void Company::updateEmployeesCompanyID(int acquirerID)
{
	all_employees_table.updateEmployeesCompanyID(acquirerID);
}

void Company::updateSalaryToEmployee(int employeeID, int new_salary)
{
	all_employees_table.find(employeeID)->setSalary(new_salary);
}

void Company::updateGradeForEmployee(int employeeID, int new_grade)
{
	all_employees_table.find(employeeID)->setGrade(new_grade);
}

bool Company::operator<(const Company& other) const
{
	return (this->id < other.id);
}

bool Company::operator>(const Company& other) const
{
	return (this->id > other.id);
}

bool Company::operator==(const Company& other) const
{
	return (this->id == other.id);
}
