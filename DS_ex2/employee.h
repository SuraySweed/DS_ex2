#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include <iostream>
#include <utility>
#include <memory>

class Employee {
private:
	int id;
	int salary;
	int grade;
	std::shared_ptr<int> company_id;

public:
	Employee(int employeeID, int employeeSalary, int employeeGrade, std::shared_ptr<int> comapnyID);
	Employee(const Employee& employeeIdData) = default;
	Employee& operator=(Employee&) = default;
	~Employee() = default;

	int getID() { return id; }
	int getSalary() { return salary; }
	int getGrade() { return grade; }
	int getCompanyID() { return *company_id; }
	std::shared_ptr<int> getCompanyIDPtr() { return company_id; }

	void setSalary(int newSalary) { salary = newSalary; }
	void setGrade(int newGrade) { grade = newGrade; }
	void setEmployerID(int newCompanyID) { *company_id = newCompanyID; }

	bool operator<(const Employee& other) const;
	bool operator<(const int& other) const;
	bool operator<=(const int& other) const;
	bool operator>(const Employee& other) const;
	bool operator>(const int& other) const;
	bool operator>=(const int& other) const;
	bool operator==(const Employee& other) const;
	bool operator==(const int& other) const;
	bool operator!=(const int& other) const;
};

#endif // EMPLOYEE_H