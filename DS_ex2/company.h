#ifndef COMPANY_H_
#define COMPANY_H_

#include <iostream>
#include "ranked_avl_tree.h"
#include "employee.h"
#include "hash_table.h"

class Company {
private:
	int id;
	int value;
	int num_of_employees_with_zero_salary;
	int sum_of_grades_of_zero_salary_employees;
	RankedAVL<Employee> employees; // only with salary != 0
	HashTable all_employees_table; // contain all the employees in the company

public:
	Company(int id, int value);
	Company(const Company& company) = default;
	Company& operator=(const Company&) = default;
	~Company() = default;

	int getCompanyID() { return id; }
	int getValue() { return value; }
	int getNumOfZeroSalaryEmployees() { return num_of_employees_with_zero_salary; }
	int getSumOfGradesOfZeroSalaryEmployees() { return sum_of_grades_of_zero_salary_employees; }
	int getNumberOfEmployeesNonZero() { return employees.getNumberOfNodes(); }
	RankedAVL<Employee>* getEmployeesTree() { return &employees; }

	bool addEmployee(Employee* employee);
	bool addEmployeeToCompanyHashTable(Employee* employee);
	bool removeEmployee(Employee* employee);
	bool removeEmployeeFromEmployeesHash(Employee* employee);
	void incZeroSalaryEmployees(Employee* employee);
	void decZeroSalaryEmployees(Employee* employee);
	void sumBumpGradesInCompany(int m, int* sum);
	void updateEmployeesCompanyID(int acquirerID);
	void updateSalaryToEmployee(int employeeID, int new_salary);
	void updateGradeForEmployee(int employeeID, int new_grade);

	bool operator<(const Company& other) const;
	bool operator>(const Company& other) const;
	bool operator==(const Company& other) const;
};

#endif // COMPANY_H