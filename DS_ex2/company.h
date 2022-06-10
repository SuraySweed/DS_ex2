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
	int owner;	// at first, owner = currentCompanyID
	RankedAVL<Employee> employees; // only with salary != 0
	HashTable all_employees_table; // contain all the employees in the company

public:
	Company(int id, int value);
	Company(Company* company);
	Company(const Company& company) = default;
	Company& operator=(const Company&) = default;
	~Company() = default;

	int getCompanyID() { return id; }
	int getValue() { return value; }
	int getNumOfZeroSalaryEmployees() { return num_of_employees_with_zero_salary; }
	int getSumOfGradesOfZeroSalaryEmployees() { return sum_of_grades_of_zero_salary_employees; }
	int getNumberOfEmployeesNonZero() { return employees.getNumberOfNodes(); }
	HashTable* getEmployeesHashTable() { return &all_employees_table; }
	RankedAVL<Employee>* getEmployeesTree() { return &employees; }
	int getNumOfEmployeesInTree() { return employees.getNumberOfNodes(); }
	int getNumberOfEmployees() { return all_employees_table.getCounter(); }
	int getOwnerID() { return owner; }
	void setNumOfZeroSalaryEmployees(int num) { num_of_employees_with_zero_salary = num; }
	void setSumOfGradeZeroSalary(int sum) { sum_of_grades_of_zero_salary_employees = sum; }
	void setOwnerID(int id) { owner = id; }
	void setEmployeesTree(RankedAVL<Employee>& other);
	void addToSumGradesZeroSalary(int added) { sum_of_grades_of_zero_salary_employees += added; }
	void fillEmployeesInArray(Employee** arr) { employees.fillArray(arr); }

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
	void deleteEmployeeFromTree();
	void deleteEmployeesFromHashTable();

	bool operator<(const Company& other) const;
	bool operator>(const Company& other) const;
	bool operator==(const Company& other) const;
};

#endif // COMPANY_H