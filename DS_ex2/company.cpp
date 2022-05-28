#include "company.h"

Company::Company(int companyID, int companyValue) : id(companyID), value(companyValue), 
	num_of_employees_with_zero_salary(0), sum_of_grades_of_zero_salary_employees(0), 
	employees(RankedAVL<Employee>()) {}

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
