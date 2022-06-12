#include "library2.h"
#include "system_manager.h"

void* Init(int k)
{
	SystemManager* DS = new SystemManager(k);
	return (void*)DS;
}

StatusType AddEmployee(void* DS, int employeeID, int companyID, int grade)
{
	if (DS == nullptr) return INVALID_INPUT;
	return ((SystemManager*)DS)->AddEmployee(employeeID, companyID, grade);
}

StatusType RemoveEmployee(void* DS, int employeeID)
{
	if (DS == nullptr) return INVALID_INPUT;
	return ((SystemManager*)DS)->RemoveEmployee(employeeID);
}

StatusType AcquireCompany(void* DS, int companyID1, int companyID2, double factor)
{
	if (DS == nullptr) return INVALID_INPUT;
	return ((SystemManager*)DS)->AcquireCompany(companyID1, companyID2, factor);
}

StatusType EmployeeSalaryIncrease(void* DS, int employeeID, int salaryIncrease)
{
	if (DS == nullptr) return INVALID_INPUT;
	return ((SystemManager*)DS)->EmployeeSalaryIncrease(employeeID, salaryIncrease);
}

StatusType PromoteEmployee(void* DS, int employeeID, int bumpGrade)
{
	if (DS == nullptr) return INVALID_INPUT;
	return ((SystemManager*)DS)->PromoteEmployee(employeeID, bumpGrade);
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void* DS, int companyID, int m)
{
	if (DS == nullptr) return INVALID_INPUT;
	return ((SystemManager*)DS)->SumOfBumpGradeBetweenTopWorkersByGroup(companyID, m);
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void* DS, int companyID, int lowerSalary, int higherSalary)
{
	if (DS == nullptr) return INVALID_INPUT;
	return ((SystemManager*)DS)->AverageBumpGradeBetweenSalaryByGroup(companyID, lowerSalary, higherSalary);
}

StatusType CompanyValue(void* DS, int companyID)
{
	if (DS == nullptr) return INVALID_INPUT;
	return ((SystemManager*)DS)->CompanyValue(companyID);
}


StatusType BumpGradeToEmployees(void* DS, int lowerSalary, int higherSalary, int bumpGrade)
{
	return StatusType();
}

void Quit(void** DS)
{
	delete ((SystemManager*)*DS);
	*DS = nullptr;
}
