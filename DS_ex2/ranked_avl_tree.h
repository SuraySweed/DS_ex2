//
// created by Boran Swaid on 31/5/2022 
//

#ifndef RANKED_AVL_TREE_H
#define RANKED_AVL_TREE_H

#include <iostream>

template<class T>
class TreeNode {
public:
	T* data;
	TreeNode* left;
	TreeNode* right;
	int sumOfGradesInSubTree;
	int rank;

	TreeNode(const T& data_t);
	TreeNode(T* data_t) : data(new T(*data_t)), left(nullptr), right(nullptr),
		sumOfGradesInSubTree(0), rank(0) {}
	TreeNode(const TreeNode<T>& node);
	TreeNode<T>& operator=(const TreeNode<T>& node);
	~TreeNode();

};

template <class T>
class RankedAVL {
	TreeNode<T>* _root;

	int max(int num1, int num2);
	TreeNode<T>* rightRotate(TreeNode<T>* root);
	TreeNode<T>* leftRotate(TreeNode<T>* root);
	TreeNode<T>* balanceTree(TreeNode<T>* root);
	TreeNode<T>* copyNodes(TreeNode<T>* root);
	void initRank(TreeNode<T>* node);
	void deleteTree(TreeNode<T>* root);

public:
	RankedAVL<T>() : _root(nullptr) {}
	RankedAVL<T>(const RankedAVL<T>& rankedTree);
	RankedAVL<T>& operator=(const RankedAVL<T>& root);
	~RankedAVL();

	int getHeight(TreeNode<T>* node);
	int getBalance(TreeNode<T>* node);
	TreeNode<T>* insertAux(TreeNode<T>* root, T* data);
	bool insert(T* data);
	TreeNode<T>* removeAux(TreeNode<T>* root, T* data);
	bool remove(T* data);
	TreeNode<T>* getRoot() { return _root; }
	TreeNode<T>* find(TreeNode<T>* root, const T& data);
	void mergeTree(T** arr1, int size1, T** arr2, int size2, T** newArr);
};

template<class T>
inline TreeNode<T>::TreeNode(const T& data_t) : data(new T(data_t)), left(nullptr), right(nullptr),
	sumOfGradesInSubTree(0), rank(0) {}

template<class T>
inline TreeNode<T>::TreeNode(const TreeNode<T>& node)
{
	data = new T(*(node.data));
	sumOfGradesInSubTree = node.sumOfGradesInSubTree;
	rank = node.rank;

	if (node.left) {
		left = new TreeNode<T>(*(node.left));
	}
	else {
		left = nullptr;
	}
	if (node.right) {
		right = new TreeNode<T>(*(node.right));
	}
	else {
		right = nullptr;
	}
}

template<class T>
inline TreeNode<T>& TreeNode<T>::operator=(const TreeNode<T>& node)
{
	if (this == &node) {
		return *this;
	}
	data = new T(*(node.data));
	if (node.left) {
		left = new TreeNode<T>(*(node.left));
	}
	else {
		left = nullptr;
	}
	if (node.right) {
		right = new TreeNode<T>(*(node.right));
	}
	else {
		right = nullptr;
	}
	return *this;
}

template<class T>
inline TreeNode<T>::~TreeNode()
{
	delete data;
}

template<class T>
inline int RankedAVL<T>::max(int num1, int num2)
{
	return num1 > num2 ? num1 : num2;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::rightRotate(TreeNode<T>* root)
{
	TreeNode<T>* root_left = root->left;
	TreeNode<T>* root_right = root_left->right;

	root_left->right = root;
	root->left = root_right;

	return root_left;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::leftRotate(TreeNode<T>* root)
{
	TreeNode<T>* root_right = root->right;
	TreeNode<T>* root_left = root_right->left;

	root_right->left = root;
	root->right = root_left;

	return root_right;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::balanceTree(TreeNode<T>* root)
{
	int balance_factor = getBalance(root);
	if (balance_factor > 1) {
		if (getBalance(root->left) < 0)
			root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	else if (balance_factor < -1) {
		if (getBalance(root->right) > 0)
			root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	return root;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::copyNodes(TreeNode<T>* root)
{
	if (root) {
		TreeNode<T>* n = new TreeNode<T>(root->data);
		n->sumOfGradesInSubTree = root->sumOfGradesInSubTree;
		n->rank = root->rank;
		n->left = copyNodes(root->left);
		n->right = copyNodes(root->right);
		return n;
	}
	else {
		return nullptr;
	}
}

template<class T>
inline void RankedAVL<T>::initRank(TreeNode<T>* node)
{
	if (!node)
		return;
	int leftR = 0, rightR = 0;
	if (node->left)
	{
		initRank(node->left);
		leftR = node->left->rank;
	}

	if (node->right)
	{
		initRank(node->right);
		rightR = node->right->rank;
	}
	node->rank = leftR + rightR + 1;
}

template<class T>
inline void RankedAVL<T>::deleteTree(TreeNode<T>* root)
{
	if (root == NULL)
		return;
	deleteTree(root->left);
	deleteTree(root->right);
	delete root;
}

template<class T>
inline RankedAVL<T>& RankedAVL<T>::operator=(const RankedAVL<T>& root)
{
	if (this == &root) {
		return *this;
	}
	deleteTree(_root);
	_root = copyNodes(root._root);
	return *this;
}

template<class T>
inline RankedAVL<T>::~RankedAVL()
{
	deleteTree(this->_root);
}

template<class T>
inline int RankedAVL<T>::getHeight(TreeNode<T>* node)
{
	int height = 0;
	if (node) {
		int leftHeight = getHeight(node->left);
		int rightHeight = getHeight(node->right);
		height = 1 + max(leftHeight, rightHeight);
	}
	return height;
}

template<class T>
inline int RankedAVL<T>::getBalance(TreeNode<T>* node)
{
	int balanceFactor = 0;
	if (node) {
		balanceFactor = getHeight(node->left) - getHeight(node->right);
	}
	return balanceFactor;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::insertAux(TreeNode<T>* root, T* data)
{
	return NULL;
}

template<class T>
inline bool RankedAVL<T>::insert(T* data)
{
	return false;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::find(TreeNode<T>* root, const T& data)
{
	if (root) {
		if (*(root->data) == data)
			return root;
		else if (*(root->data) > data)
			return find(root->left, data);
		else
			return find(root->right, data);
	}

	return nullptr;
}

template<class T>
inline RankedAVL<T>::RankedAVL(const RankedAVL<T>& rankedTree)
{
	_root = copyNodes(rankedTree._root);
}

#endif // RANKED_AVL_TREE_H_