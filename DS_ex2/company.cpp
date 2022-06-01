#include "company.h"

Company::Company(int companyID, int companyValue) : id(companyID), value(companyValue), 
	num_of_employees_with_zero_salary(0), sum_of_grades_of_zero_salary_employees(0), 
	employees(RankedAVL<Employee>()) {}

bool Company::addEmployee(Employee* employee)
{
	if (employees.insert(employee)) {
		return true;
	}
	return false;
}

bool Company::removeEmployee(Employee* employee)
{
	TreeNode<Employee>* returnedNode = employees.remove(employee); // return nullptr if there is one node, and we remove it3
	int company_employees_number = employees.getNumberOfNodes();
	
	if ((!returnedNode && company_employees_number == 1) || (returnedNode && company_employees_number > 1)) { 
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
