#include "employee.h"
#include "company.h"

Employee::Employee(int employeeID, int employeeSalary, int employeeGrade, std::shared_ptr<int> comapnyID) :
	id(employeeID), salary(employeeSalary), grade(employeeGrade), company_id(std::move(comapnyID)) {}


bool Employee::operator<(const Employee& other) const
{
	if (this->salary < other.salary) {
		return true;
	}
	else if (this->salary == other.salary) {
		return (this->id < other.id);
	}
	return false;
}

bool Employee::operator<(const int& other) const
{
	return (this->salary < other);
}

bool Employee::operator<=(const int& other) const
{
	return (this->salary <= other);
}

bool Employee::operator>(const Employee& other) const
{
	if (this->salary > other.salary) {
		return true;
	}
	else if (this->salary == other.salary) {
		return (this->id > other.id);
	}
	return false;
}

bool Employee::operator>(const int& other) const
{
	return (this->salary > other);
}

bool Employee::operator>=(const int& other) const
{
	return (this->salary >= other);
}

bool Employee::operator==(const Employee& other) const
{
	return((this->salary == other.salary) && (this->id == other.id));
}

bool Employee::operator==(const int& other) const
{
	return(this->salary == other);
}

bool Employee::operator!=(const int& other) const
{
	return (this->salary != other);
}
