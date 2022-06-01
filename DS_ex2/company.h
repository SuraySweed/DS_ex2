#ifndef COMPANY_H_
#define COMPANY_H_

#include <iostream>
#include "ranked_avl_tree.h"
#include "employee.h"

class Company {
private:
	int id;
	int value;
	int num_of_employees_with_zero_salary;
	int sum_of_grades_of_zero_salary_employees;
	RankedAVL<Employee> employees;

public:
	Company(int id, int value);
	Company(const Company& company) = default;
	Company& operator=(const Company&) = default;
	~Company() = default;

	int getCompanyID() { return id; }
	int getValue() { return value; }
	int getNumOfZeroSalaryEmployees() { return num_of_employees_with_zero_salary; }
	int getSumOfGradesOfZeroSalaryEmployees() { return sum_of_grades_of_zero_salary_employees; }
	
	bool addEmployee(Employee* employee);
	bool removeEmployee(Employee* employee);
	void incZeroSalaryEmployees(Employee* employee);
	void decZeroSalaryEmployees(Employee* employee);

	bool operator<(const Company& other) const;
	bool operator>(const Company& other) const;
	bool operator==(const Company& other) const;
};

#endif // COMPANY_H