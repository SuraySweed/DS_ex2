#ifndef INVERTTED_TREE_H_
#define INVERTTED_TREE_H_

#include <iostream>
#include "company.h"
#include "ranked_avl_tree.h"
#include "hash_table.h"

class InvertedTree {
private:
	int key;
	Company* data;
	int size;
	double acquired_value;
	InvertedTree* next;

	void updatePath(InvertedTree* head, InvertedTree* node);
	void updateAcquiredValue(InvertedTree* node);

public:
	InvertedTree(int key, Company* data) : key(key), data(data), size(1), acquired_value(0), next(nullptr) {}
	InvertedTree(const InvertedTree& invertedTree) = default;
	InvertedTree& operator=(const InvertedTree& invertedTree) = default;
	~InvertedTree();
	
	Company* getData() { return data; }
	double getAcquiredValue() { return acquired_value; }
	InvertedTree* find(InvertedTree* group);
	void Union(InvertedTree* group1, InvertedTree* group2, double factor);
	void fillGradesArray(Employee** employees_arr, int grades[], int size);
	void mergeCompaniesTrees(Company* acquirerCompany, Company* targetCompany);
	void mergeCompaniesHashies(Company* acquirerCompany, Company* targetCompany);
};

inline void InvertedTree::updatePath(InvertedTree* head, InvertedTree* node)
{
	//InvertedTree* node = group;
	if (head == node) {
		return;
	}

	InvertedTree* temp;
	while (node->next) {
		temp = node->next;
		node->next = head;
		node = temp;
	}
}

inline void InvertedTree::updateAcquiredValue(InvertedTree* node)
{
	if (node->next == nullptr) {
		return;
	}
	updateAcquiredValue(node->next);
	if (node->next->next == nullptr) {
		return;
	}
	else {
		node->acquired_value += node->next->acquired_value;
	}
}

inline InvertedTree::~InvertedTree()
{
	while (next) {
		InvertedTree* toDelete = next;
		next = next->next;
		delete toDelete;
	}
}

inline InvertedTree* InvertedTree::find(InvertedTree* group)
{
	InvertedTree* head = group;
	while (head->next) {
		head = head->next;
	}
	
	updateAcquiredValue(group);
	updatePath(head, group);

	return head;
}

inline void InvertedTree::Union(InvertedTree* group1, InvertedTree* group2, double factor)
{
	auto parent1 = find(group1);
	auto parent2 = find(group2);

	if (parent1 == parent2) {
		return;
	}

	// parent1 is the acquierer company
	if (parent1->size > parent2->size) {
		parent1->acquired_value += factor * (parent2->getData()->getValue() + parent2->acquired_value);
		parent2->acquired_value -= parent1->acquired_value;
		parent2->next = parent1;
		parent1->size += parent2->size;
		parent1->data->setNumOfZeroSalaryEmployees(parent1->data->getNumOfZeroSalaryEmployees() +
			parent2->data->getNumOfZeroSalaryEmployees());
		parent1->data->setSumOfGradeZeroSalary(parent1->data->getSumOfGradesOfZeroSalaryEmployees() +
			parent2->data->getSumOfGradesOfZeroSalaryEmployees());

		parent2->getData()->setOwnerID(parent1->getData()->getCompanyID()); // set ownerID to the target company

		if (parent2->getData()->getNumberOfEmployees() > 0) {
			if (parent2->getData()->getNumOfEmployeesInTree() > 0) {
				mergeCompaniesTrees(parent1->getData(), parent2->getData());
			}
			mergeCompaniesHashies(parent1->getData(), parent2->getData());
		}
	}
	// parent2 is the acquirer fucntion
	else {
		parent1->acquired_value += factor * (parent2->getData()->getValue() + parent2->acquired_value);
		parent1->acquired_value -= parent2->acquired_value;
		parent1->next = parent2;
		parent2->size += parent1->size;
		parent2->data->setNumOfZeroSalaryEmployees(parent1->data->getNumOfZeroSalaryEmployees() +
			parent2->data->getNumOfZeroSalaryEmployees());
		parent2->data->setSumOfGradeZeroSalary(parent1->data->getSumOfGradesOfZeroSalaryEmployees() +
			parent2->data->getSumOfGradesOfZeroSalaryEmployees());

		parent1->getData()->setOwnerID(parent2->getData()->getCompanyID());

		if (parent1->getData()->getNumberOfEmployees() > 0) {
			if (parent1->getData()->getNumOfEmployeesInTree() > 0) {
				mergeCompaniesTrees(parent2->getData(), parent1->getData());
			}
			mergeCompaniesHashies(parent2->getData(), parent1->getData());
		}
	}
}

inline void InvertedTree::fillGradesArray(Employee** employees_arr, int grades[], int size)
{
	for (int i = 0; i < size; i++) {
		grades[i] = employees_arr[i]->getGrade();
	}
}

inline void InvertedTree::mergeCompaniesTrees(Company* acquirerCompany, Company* targetCompany)
{
	if (acquirerCompany->getNumOfEmployeesInTree() == 0) {
		acquirerCompany->setEmployeesTree(*(targetCompany->getEmployeesTree()));
	}

	else {
		int target_employees_number = targetCompany->getNumOfEmployeesInTree();
		int* target_grades = new int[target_employees_number];
		Employee** target_employees_arr = new Employee * [target_employees_number];
		int acquirer_employees_number = acquirerCompany->getNumOfEmployeesInTree();
		int* acquirer_grades = new int[acquirer_employees_number];
		Employee** acquirer_employees_arr = new Employee * [acquirer_employees_number];

		targetCompany->fillEmployeesInArray(target_employees_arr);
		fillGradesArray(target_employees_arr, target_grades, target_employees_number);
		acquirerCompany->fillEmployeesInArray(acquirer_employees_arr);
		fillGradesArray(acquirer_employees_arr, acquirer_grades, acquirer_employees_number);

		int total_employees = target_employees_number + acquirer_employees_number;
		int* total_grades = new int[total_employees];
		Employee** total_employees_arr = new Employee * [total_employees];

		Employee** target_fill_array = new Employee * [target_employees_number];
		Employee** acquire_fill_array = new Employee * [acquirer_employees_number];

		for (int i = 0; i < target_employees_number; i++) {
			target_fill_array[i] = new Employee(*(target_employees_arr[i]));
		}

		for (int i = 0; i < acquirer_employees_number; i++) {
			acquire_fill_array[i] = new Employee(*(acquirer_employees_arr[i]));
		}

		acquirerCompany->getEmployeesTree()->mergeTree(target_fill_array, target_grades, target_employees_number,
			acquire_fill_array, acquirer_grades, acquirer_employees_number, total_employees_arr, total_grades);

		//delete targetCompany->getEmployeesTree();
		targetCompany->deleteEmployeeFromTree();

		for (int i = 0; i < target_employees_number; i++) {
			delete target_fill_array[i];
		}

		for (int i = 0; i < acquirer_employees_number; i++) {
			delete acquire_fill_array[i];
		}

		delete[] target_fill_array;
		delete[] acquire_fill_array;
	}
}

inline void InvertedTree::mergeCompaniesHashies(Company* acquirerCompany, Company* targetCompany)
{
	acquirerCompany->getEmployeesHashTable()->mergeTwoHashies(acquirerCompany->getEmployeesHashTable(),
		targetCompany->getEmployeesHashTable());
}


#endif
