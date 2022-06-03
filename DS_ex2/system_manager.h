#ifndef SYSTEM_MANAGER_H_
#define SYSTEM_MANAGER_H_

#include <iostream>
#include "ranked_avl_tree.h"
#include "inverted_tree.h"
#include "hash_table.h"
#include "company.h"
#include "employee.h"
#include "library2.h"

class SystemManager {
private:

	int number_of_companies; //k
	int number_of_employees;

	InvertedTree<Company*>** companies;
	HashTable* employeesTable;
	RankedAVL<Employee>* employeesTree;

	bool removeEmployeeeFromEmployeesTree(Employee* employee);
	bool removeEmployeeeFromTheCompany(Employee* employee, int company_id);
	void decZeroSalaryDataInCompany(Employee* employee, int company_id);
	bool insertEmployeeToTreeAndCompany(Employee* employee, int company_id);
	Company* getCompany(int company_id);

public:
	SystemManager(int k);
	SystemManager(const SystemManager& system_manager) = default;
	~SystemManager(); 

	StatusType AddEmployee(int employeeID, int companyID, int grade);
	StatusType RemoveEmployee(int employeeID);
	StatusType AcquireCompany(int companyID1, int companyID2, double factor);
	StatusType EmployeeSalaryIncrease(int employeeID, int salaryIncrease);
	StatusType PromoteEmployee(int employeeID, int bumpGrade);
	StatusType SumOfBumpGradeBetweenTopWorkersByGroup(int companyID, int m, void* sumBumpGrade);
	StatusType AverageBumpGradeBetweenSalaryByGroup(int companyID, int lowerSalary, int higherSalary, void* averageBumpGrade);
	StatusType CompanyValue(int companyID, void* standing);

};

#endif
