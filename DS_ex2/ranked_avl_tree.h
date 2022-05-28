#ifndef RANKED_AVL_TREE_H
#define RANKED_AVL_TREE_H

#include <iostream>
#include "node.h"

template <class T>
class RankedAVL {
	Node<T>* _root;
public:
	RankedAVL<T>();
	RankedAVL<T>(const RankedAVL<T>& rankedTree);
	RankedAVL<T>& operator=(const RankedAVL<T>& root);
	~RankedAVL();

	Node<T>* insertAux(Node<T>* root, T* data);
	bool insert(T* data);
	Node<T>* getRoot() { return _root; }
	Node<T>* findAux(Node<T>* root, const T& data);
	bool find(const T& data);
};

#endif // RANKED_AVL_TREE_H_