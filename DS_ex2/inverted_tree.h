#ifndef INVERTTED_TREE_H_
#define INVERTTED_TREE_H_

#include <iostream>

template <class T>
class InvertedTreeNode {
private:
	int key;
	T data;
	int size;
	int acquired_value;
	InvertedTreeNode* next;

public:
	InvertedTreeNode(int key, T data) : key(key), data(data), size(1), acquired_value(0), next(nullptr) {}
	InvertedTreeNode(const InvertedTreeNode<T>& invertedTreeNode) = default;
	InvertedTreeNode<T>& operator=(const InvertedTreeNode<T>&) = default;
	~InvertedTreeNode() = default;

	T getData() { return data; }
};


template <class T>
class InvertedTree {
private:
	InvertedTreeNode<T>* node;

public:
	InvertedTree(int key, T data) : node(new InvertedTreeNode<T>(key, data)) {}
	InvertedTree(const InvertedTree<T>& invertedTree) = default;
	InvertedTree<T>& operator=(const InvertedTree<T>& invertedTree) = default;
	~InvertedTree();
	
	InvertedTreeNode<T>* find(InvertedTree<T>* leaf);
	void unionFun(InvertedTree<T>* leaf, InvertedTree<T>* secondLeaf);
	//InvertedTreeNode<T>* getRoot(InvertedTreeNode<T>* leaf);
};

template<class T>
inline InvertedTree<T>::~InvertedTree()
{
	while (node) {
		InvertedTreeNode<T>* toDelete = node;
		node = node->next;
		delete toDelete;
	}
}

template<class T>
inline InvertedTreeNode<T>* InvertedTree<T>::find(InvertedTree<T>* leaf)
{
	InvertedTreeNode<T>* head = leaf->node;
	while (head->next) {
		head = head->next;
	}

	InvertedTreeNode<T>* node = leaf;
	InvertedTreeNode<T>* temp;
	while (node->next) {
		temp = node->next;
		node->next = head;
		node = temp;
	}
	return head;
}

template<class T>
inline void InvertedTree<T>::unionFun(InvertedTree<T>* leaf, InvertedTree<T>* secondLeaf)
{
	auto leaf1 = find(leaf->node);
	auto leaf2 = find(secondLeaf->node);
	leaf2->next = leaf1;
	leaf1->size += leaf2->size;
	leaf1->next = nullptr;
}


#endif
