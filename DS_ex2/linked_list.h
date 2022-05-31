#ifndef NODE_H_
#define NODE_H_

#include <iostream>
#include "emplyee.h"

class Node {
public:
	Node(const Employee& employee);
	Node(const Node& node);
	Node& operator=(const Node& node);
	~Node();

	Employee* data;
	Node* next;
};

class LinkedList {
private:
	Node* head;
	int size;

public:
	LinkedList();
	LinkedList(const LinkedList& linked_list);
	LinkedList& operator=(const LinkedList& linked_list);
	~LinkedList();

	void insert(const Employee& employee);
	void remove(const Employee& employee);
	Node* find(const Employee& employee);
	Employee* getEmployeeData(Node* node);
	void setHead(Node* newHead);
	Node* getHead() { return head; }
};

#endif //NODE_H