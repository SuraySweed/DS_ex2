#include "library2.h"
#include "system_manager.h"

void* Init(int k)
{
	SystemManager* DS = new SystemManager(k);
	return (void*)DS;
}

StatusType AddEmployee(void* DS, int employeeID, int companyID, int grade)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((SystemManager*)DS)->AddEmployee(employeeID, companyID, grade);
}

StatusType RemoveEmployee(void* DS, int employeeID)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((SystemManager*)DS)->RemoveEmployee(employeeID);
}

StatusType AcquireCompany(void* DS, int companyID1, int companyID2, double factor)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((SystemManager*)DS)->AcquireCompany(companyID1, companyID2, factor);
}

StatusType EmployeeSalaryIncrease(void* DS, int employeeID, int salaryIncrease)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((SystemManager*)DS)->EmployeeSalaryIncrease(employeeID, salaryIncrease);
}

StatusType PromoteEmployee(void* DS, int employeeID, int bumpGrade)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((SystemManager*)DS)->PromoteEmployee(employeeID, bumpGrade);
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void* DS, int companyID, int m, void* sumBumpGrade)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((SystemManager*)DS)->SumOfBumpGradeBetweenTopWorkersByGroup(companyID, m, sumBumpGrade);
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void* DS, int companyID, int lowerSalary, int higherSalary, void* averageBumpGrade)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((SystemManager*)DS)->AverageBumpGradeBetweenSalaryByGroup(companyID, lowerSalary, higherSalary, averageBumpGrade);
}

StatusType CompanyValue(void* DS, int companyID, void* standing)
{
	if (DS == NULL) return INVALID_INPUT;
	return ((SystemManager*)DS)->CompanyValue(companyID, standing);
}


void Quit(void** DS)
{
	delete ((SystemManager*)*DS);
	*DS = nullptr;
}
