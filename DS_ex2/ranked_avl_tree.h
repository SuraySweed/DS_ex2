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
	int Grade;
	int sumOfGrades;
	int rank;
	//need to add numberOfChildren for the rank purpose

	TreeNode(const T& data_t);
	TreeNode(T* data_t) : data(new T(*data_t)), left(nullptr), right(nullptr),
		Grade(0), sumOfGrades(0), rank(1) {}
	TreeNode(const TreeNode<T>& node);
	TreeNode<T>& operator=(const TreeNode<T>& node);
	~TreeNode();

};

template <class T>
class RankedAVL {
	TreeNode<T>* _root;
	int NodesNumber;

	int max(int num1, int num2);
	TreeNode<T>* rightRotate(TreeNode<T>* root);
	TreeNode<T>* leftRotate(TreeNode<T>* root);
	TreeNode<T>* balanceTree(TreeNode<T>* root);
	TreeNode<T>* copyNodes(TreeNode<T>* root);
	//void initRank(TreeNode<T>* node);
	void deleteTree(TreeNode<T>* root);
	//int rank(TreeNode<T>* root, const T* data);
	TreeNode<T>* selectAux(TreeNode<T>* root, int k, int& r);

public:
	RankedAVL<T>() : _root(nullptr), NodesNumber(0) {}
	RankedAVL<T>(const RankedAVL<T>& rankedTree);
	RankedAVL<T>& operator=(const RankedAVL<T>& root);
	~RankedAVL();

	int getHeight(TreeNode<T>* node);
	int getBalance(TreeNode<T>* node);
	int getNodesNumber();
	//TreeNode<T>* insertAux(TreeNode<T>* root, T* data);
	TreeNode<T>* insert(TreeNode<T>* root, T* data, int grade, int sumOfGrade);
	//TreeNode<T>* removeAux(TreeNode<T>* root, T* data);
	TreeNode<T>* remove(TreeNode<T>* root, T* data, int grade);
	void initRank(TreeNode<T>* node);
	TreeNode<T>* getRoot() { return _root; }
	TreeNode<T>* find(TreeNode<T>* root, const T& data);
	TreeNode<T>* select(TreeNode<T>* root, int k);
	void mergeTree(T** arr1, int size1, T** arr2, int size2, T** newArr);
	TreeNode<T>* findMinNodeInSubTree(TreeNode<T>* subTreeRoot);
};

template<class T>
inline TreeNode<T>::TreeNode(const T& data_t) : data(new T(data_t)), left(nullptr), right(nullptr),
Grade(0), sumOfGrades(0), rank(0) {}

template<class T>
inline TreeNode<T>::TreeNode(const TreeNode<T>& node)
{
	data = new T(*(node.data));
	Grade = node.Grade;
	sumOfGrades = node.sumOfGrades;
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
	root_left->sumOfGrades += root->sumOfGrades;
	root->left = root_right;
	if (root_right)
		root_right->sumOfGrades += root->Grade;

	return root_left;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::leftRotate(TreeNode<T>* root)
{
	TreeNode<T>* root_right = root->right;
	TreeNode<T>* root_left = root_right->left;

	root_right->left = root;
	root->sumOfGrades -= root_right->sumOfGrades;
	root->right = root_left;
	if (root_left)
		root_left->sumOfGrades -= root_right->Grade;

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
		n->sumOfGrades = root->sumOfGrades;
		n->rank = root->rank;
		n->Grade = root->Grade;
		n->left = copyNodes(root->left);
		n->right = copyNodes(root->right);
		return n;
	}
	else {
		return nullptr;
	}
}

template<class T>
inline void RankedAVL<T>::initRank(TreeNode<T>* node)//need to fix this function
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
inline TreeNode<T>* RankedAVL<T>::select(TreeNode<T>* root, int k)
{
	int r = -1;
	TreeNode<T>* tmp = selectAux(root, k, r);
	return tmp;
}


template<class T>
inline TreeNode<T>* RankedAVL<T>::selectAux(TreeNode<T>* root, int k, int& r)
{
	if (!root)
		return nullptr;
	if (root->left)
	{
		if (k < root->left->rank + 1 + r)
			return selectAux(root->left, k, r);
		else if (k > root->left->rank + 1 + r)
		{
			r += root->left->rank + 1;
			return selectAux(root->right, k, r);
		}
		else
			return root;
	}
	else
	{
		if (k == r + 1)
		{
			return root;
		}
		r++;
		return selectAux(root->right, k, r);
	}
}

template<class T>
inline RankedAVL<T>& RankedAVL<T>::operator=(const RankedAVL<T>& root)
{
	if (this == &root) {
		return *this;
	}
	deleteTree(_root);
	_root = copyNodes(root._root);
	NodesNumber = root.NodesNumber;
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
inline int RankedAVL<T>::getNodesNumber()
{
	return NodesNumber;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::insert(TreeNode<T>* root, T* data, int grade, int sumOfGrade)
{
	if (root == nullptr) {
		TreeNode<T>* new_root = new TreeNode<T>(*data);
		new_root->Grade = grade;
		new_root->sumOfGrades = sumOfGrade;
		if (_root == nullptr) {
			_root = new_root;
		}
		NodesNumber++;
		return new_root;
	}

	if (root && (*(root->data) > * data)) {
		sumOfGrade += root->Grade;
		root->left = insert(root->left, data, grade, sumOfGrade);
	}
	else if (root && (*(root->data) < *data)) {
		root->sumOfGrades += grade;
		root->right = insert(root->right, data, grade, sumOfGrade);
	}
	else {
		return root;
	}

	root = balanceTree(root);
	_root = root;
	return root;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::remove(TreeNode<T>* root, T* data, int grade)
{
	if (root == nullptr) {
		return root;
	}

	if (*(root->data) > * data) {
		root->left = remove(root->left, data, grade);
	}

	else if (*(root->data) < *data) {
		root->sumOfGrades -= grade;
		root->right = remove(root->right, data, grade);
	}

	else {
		if ((root->left == nullptr) || (root->right == nullptr)) {
			TreeNode<T>* temp = root->left ? root->left : root->right;
			if (temp == nullptr) {
				temp = root;
				root = nullptr;
				_root = root;
			}
			else {
				if (temp == root->right) root->sumOfGrades = temp->sumOfGrades;
				*(root) = *(temp);
				root->Grade = temp->Grade;

			}
			NodesNumber--;
			delete temp;
		}
		else {
			TreeNode<T>* temp = findMinNodeInSubTree(root->right);
			root->left->sumOfGrades += (temp->Grade - root->Grade);
			*(root->data) = *(temp->data);
			root->Grade = temp->Grade;
			root->sumOfGrades -= temp->Grade;
			root->left->sumOfGrades += (temp->Grade - root->Grade);
			root->right = remove(root->right, temp->data, grade);
		}
	}

	if (root == nullptr) {
		return root;
	}
	root = balanceTree(root);
	_root = root;
	return root;
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
inline TreeNode<T>* RankedAVL<T>::findMinNodeInSubTree(TreeNode<T>* subTreeRoot)
{
	while (subTreeRoot && subTreeRoot->left) {
		subTreeRoot = subTreeRoot->left;
	}
	return subTreeRoot;
}

template<class T>
inline RankedAVL<T>::RankedAVL(const RankedAVL<T>& rankedTree)
{
	_root = copyNodes(rankedTree._root);
	this->NodesNumber = rankedTree.NodesNumber;
}

#endif // RANKED_AVL_TREE_H_