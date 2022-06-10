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
	
	TreeNode(const T& data_t);
	TreeNode(T* data_t) : data(new T(*data_t)), left(nullptr), right(nullptr),
		Grade(0), sumOfGrades(0), rank(0) {}
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
	void deleteTree(TreeNode<T>* root);
	TreeNode<T>* selectAux(TreeNode<T>* root, int k, int& r);
	void mergeArray(T** arr1, int grades1[], int size1, T** arr2, int grades2[], int size2, T** newArr, int grades[]);
	TreeNode<T>* doMergerTree(T** arr1, int grades1[], int size1, T** arr2, int grades2[], int size2, T** newArr, int grades[]);
	TreeNode<T>* auxBuildTree(int left, int right, TreeNode<T>* root, T** newArr, int grades[]);
	void inorderArray(TreeNode<T>* root, T** arr, int& i);
	TreeNode<T>* removeAux(TreeNode<T>* root, T* data, int grade);
	TreeNode<T>* insertAux(TreeNode<T>* root, T* data, int grade, int sumOfGrade, int rank);
	void sumBumpGradeAux(TreeNode<T>* root, int m, int* sum);
	TreeNode<T>* getLastInIntervalAux(TreeNode<T>* root, int high, TreeNode<T>* last);
	TreeNode<T>* getFirstInIntervalAux(TreeNode<T>* root, int low, TreeNode<T>* first);

	
public:
	RankedAVL<T>() : _root(nullptr), NodesNumber(0) {}
	RankedAVL<T>(const RankedAVL<T>& rankedTree);
	RankedAVL<T>& operator=(const RankedAVL<T>& root);
	~RankedAVL();

	int getHeight(TreeNode<T>* node);
	int getBalance(TreeNode<T>* node);
	int getNumberOfNodes();
	TreeNode<T>* insert(T* data, int grade);
	TreeNode<T>* remove(T* data, int grade);
	TreeNode<T>* getRoot() { return _root; }
	TreeNode<T>* find(TreeNode<T>* root, const T& data);
	TreeNode<T>* select(TreeNode<T>* root, int k);
	void fillArray(T* arr[]);
	void mergeTree(T** arr1, int grades1[], int size1, T** arr2, int grades2[], int size2, T** newArr, int grades[]);
	TreeNode<T>* findMinNodeInSubTree(TreeNode<T>* subTreeRoot);
	void sumBumpGrade(int m, int* sum);
	//void getEmployeesDataBetweenHighLow(int low, int high, int* Number, int* sum);
	T* getMinNodeData(TreeNode<T>* root);
	T* getMaxNodeData(TreeNode<T>* root);
	TreeNode<T>* getLastInInterval(TreeNode<T>* root, int high);
	TreeNode<T>* getFirstInInterval(TreeNode<T>* root, int low);
	void calcRank(TreeNode<T>* root, TreeNode<T>* node, int* rank);
	void calcSumOfGrades(TreeNode<T>* root, TreeNode<T>* node, int* sum);
	void deleteNodes(TreeNode<T>* root);
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
	Grade = node.Grade;//////////////
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
	root_left->sumOfGrades = root->sumOfGrades + root->Grade;
	root_left->rank = root->rank + 1;
	root->left = root_right;
	/*if (root_right) {
		root_right->sumOfGrades += root->Grade;
		root_right->rank += 1;
	}*/

	return root_left;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::leftRotate(TreeNode<T>* root)
{
	TreeNode<T>* root_right = root->right;
	TreeNode<T>* root_left = root_right->left;

	root_right->left = root;
	root->sumOfGrades -= (root_right->sumOfGrades + root_right->Grade);
	root->rank -= (root_right->rank + 1);
	root->right = root_left;
	if (root_left) {
		root->sumOfGrades = root_left->Grade + root_left->sumOfGrades;
		root->rank = 1 + root_left->rank;
	}

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

/*template<class T>
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

}*/

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
inline void RankedAVL<T>::fillArray(T* arr[])
{
	int i = 0;
	inorderArray(_root, arr, i);
}

template<class T>
inline void RankedAVL<T>::mergeTree(T** arr1, int grades1[], int size1, T** arr2, int grades2[], int size2, T** newArr, int grades[])
{
	_root = doMergerTree(arr1, grades1, size1, arr2, grades2, size2, newArr, grades);
	NodesNumber = size1 + size2;
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
inline void RankedAVL<T>::mergeArray(T** arr1, int grades1[], int size1, T** arr2, int grades2[], int size2, T** newArr, int grades[])
{
	int i = 0, j = 0, k = 0;
	while (i < size1 && j < size2) {
		if (*arr1[i] < *arr2[j]) {
			newArr[k] = arr1[i];
			grades[k] = grades1[i];
			i++;
			k++;
		}
		else {
			newArr[k] = arr2[j];
			grades[k] = grades2[j];
			j++;
			k++;
		}
	}
	if (i >= size1) {
		while (j < size2) {
			newArr[k] = arr2[j];
			grades[k] = grades2[j];
			j++;
			k++;
		}
	}
	if (j >= size2) {
		while (i < size1) {
			newArr[k] = arr1[i];
			grades[k] = grades1[i];
			i++;
			k++;
		}
	}
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::doMergerTree(T** arr1, int grades1[], int size1, T** arr2, int grades2[], int size2, T** newArr, int grades[])
{
	mergeArray(arr1, grades1, size1, arr2, grades2, size2, newArr, grades);
	TreeNode<T>* root = nullptr;
	int left = 0;
	int right = size1 + size2 - 1;
	TreeNode<T>* newRoot = auxBuildTree(left, right, root, newArr, grades);
	return newRoot;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::auxBuildTree(int left, int right, TreeNode<T>* root, T** newArr, int grades[])
{
	if (right < left)
		return nullptr;
	root = new TreeNode<T>(new T(*(newArr[((right + left) / 2)])));
	root->Grade = grades[((right + left) / 2)];
	root->left = auxBuildTree(left, ((right + left) / 2) - 1, root->left, newArr, grades);
	if (root->left) {
		root->left->rank += 1;
		root->left->sumOfGrades += root->Grade;
	}
	root->right = auxBuildTree(((left + right) / 2) + 1, right, root->right, newArr, grades);

	root->rank = root->right ? root->right->rank + 1 : 0;
	root->sumOfGrades = root->right ? (root->right->sumOfGrades + root->right->Grade) : 0;
	if (root->right && root->right->left) {
		root->rank += root->right->left->rank + 1;
		root->sumOfGrades += root->right->left->sumOfGrades + root->right->left->Grade - root->right->Grade;
	}
	return root;
}

template<class T>
inline void RankedAVL<T>::inorderArray(TreeNode<T>* root, T** arr, int& i)
{
	if (root == nullptr)
		return;
	inorderArray(root->left, arr, i);
	arr[i] = (root->data);
	i++;
	inorderArray(root->right, arr, i);
	return;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::removeAux(TreeNode<T>* root, T* data, int grade)
{
	if (root == nullptr) {
		return root;
	}

	if (*(root->data) > *data) {
		root->left = removeAux(root->left, data, grade);
	}

	else if (*(root->data) < *data) {
		root->sumOfGrades -= grade;
		root->rank--;
		root->right = removeAux(root->right, data, grade);
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
				if (temp == root->right) {
					root->sumOfGrades -= temp->Grade;///////////////changed
					root->rank--;
				}
				*(root) = *(temp);
				root->Grade = temp->Grade;

			}
			NodesNumber--;
			delete temp;
		}
		else {
			TreeNode<T>* temp = findMinNodeInSubTree(root->right);
			//root->left->sumOfGrades += (temp->Grade - root->Grade);
			*(root->data) = *(temp->data);
			root->Grade = temp->Grade;
			root->sumOfGrades -= temp->Grade;
			root->rank--;
			//root->left->sumOfGrades += (temp->Grade - root->Grade);/////maybe need to dic rank
			root->right = removeAux(root->right, temp->data, grade);
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
inline TreeNode<T>* RankedAVL<T>::insertAux(TreeNode<T>* root, T* data, int grade, int sumOfGrade, int rank)
{
	if (root == nullptr) {
		TreeNode<T>* new_root = new TreeNode<T>(*data);
		new_root->Grade = grade;
		new_root->sumOfGrades = sumOfGrade;
		new_root->rank = rank;
		if (_root == nullptr) {
			_root = new_root;
		}
		NodesNumber++;
		return new_root;
	}

	if (root && (*(root->data) > *data)) {
		//sumOfGrade = root->Grade;//// = in +=
		//rank = 1;
		root->left = insertAux(root->left, data, grade, sumOfGrade, rank);
	}
	else if (root && (*(root->data) < *data)) {
		root->sumOfGrades += grade;
		root->rank++;
		root->right = insertAux(root->right, data, grade, sumOfGrade, rank);
	}
	else {
		return root;
	}

	root = balanceTree(root);
	_root = root;
	return root;
}

template<class T>
inline void RankedAVL<T>::sumBumpGradeAux(TreeNode<T>* root, int m, int* sum)
{
	if (!root || m == 0) {
		return;
	}
	
	if (root->rank == m) {
		*sum += root->sumOfGrades;
		return;
	}
	else if (root->rank > m) {
		sumBumpGradeAux(root->right, m, sum);
	}
	else {
		m -= (root->rank + 1);
		*sum += root->sumOfGrades + root->Grade;
		/*if (m != 0) {
			*sum -= root->Grade;
			m += 1;
		}*/
		sumBumpGradeAux(root->left, m, sum);
		
	}
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::getLastInIntervalAux(TreeNode<T>* root, int high, TreeNode<T>* last)
{
	return NULL;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::getFirstInIntervalAux(TreeNode<T>* root, int low, TreeNode<T>* first)
{
	if (root == nullptr) {
		return first;
	}

	if (*root->data >= low) {
		if (first == nullptr){// && (*root->data != low)) {
			first = root;
		}
		if ((first) && (*first->data > *root->data || *first->data == *root->data)) {//&& (*root->data != low)) {
			first = root;
			return getFirstInIntervalAux(root->left, low, first);
		}
		else {
			return getFirstInIntervalAux(root->right, low, first);
		}
	}
	else {
		return getFirstInIntervalAux(root->right, low, first);
	}
}

template<class T>
inline T* RankedAVL<T>::getMinNodeData(TreeNode<T>* root)
{
	while (root && root->left) {
		root = root->left;
	}
	if (root) return root->data;
	return nullptr;
}

template<class T>
inline T* RankedAVL<T>::getMaxNodeData(TreeNode<T>* root)
{
	while (root && root->right) {
		root = root->right;
	}
	if (root) return root->data;
	return nullptr;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::getLastInInterval(TreeNode<T>* root, int high)
{
	if (root && (*(root->data) == high) && (root->right && (*(root->right->data) != high))) {
		return root;
	}

	else if (root && (*(root->data) > high)) {
		//if (!(root->left)) return root;
		return getLastInInterval(root->left, high);
	}
	
	else if (root && (*(root->data) <= high)) {
		if (!(root->right) || (*(root->right->data) > high && (!(root->right->left) || *(root->right->left->data) > high)))
			return root;
		return getLastInInterval(root->right, high);
	}

}

template<class T>
inline TreeNode<T>* RankedAVL<T>::getFirstInInterval(TreeNode<T>* root, int low)
{
	TreeNode<T>* first = nullptr;
	first = getFirstInIntervalAux(root, low, first);
	return first;
	/*if (root && (*(root->data) == low) && (root->left && (*(root->left->data) != low))) {
		return root;
	} 

	else if (root && (*(root->data) < low)) {
		return getFirstInInterval(root->right, low);
	} 

	else if (root && (*(root->data) >= low)) {
		if (!(root->left) || (*(root->left->data) < low && (!root->left->right || *(root->left->right->data) < low)))
			return root;
		return getFirstInInterval(root->left, low);
	}*/

}

template<class T>
inline void RankedAVL<T>::calcRank(TreeNode<T>* root, TreeNode<T>* node, int* rank)
{
	if (*(root->data) == *(node->data)) {
		//*rank += 1;
		*rank += node->rank;
		return;
	}
	else if (*(root->data) > *(node->data)) {
		*rank += (root->rank + 1);
		calcRank(root->left, node, rank);
	}
	else {
		calcRank(root->right, node, rank);
	}
}

template<class T>
inline void RankedAVL<T>::calcSumOfGrades(TreeNode<T>* root, TreeNode<T>* node, int* sum)
{
	if (*(root->data) == *(node->data)) {
		//*sum += root->Grade;
		*sum += node->sumOfGrades;
		return;
	}
	else if (*(root->data) > *(node->data)) {
		*sum += (root->sumOfGrades + root->Grade);
		//if (!root->left) *sum += root->Grade;
		calcSumOfGrades(root->left, node, sum);
	}
	else {
		calcSumOfGrades(root->right, node, sum);
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
inline int RankedAVL<T>::getNumberOfNodes()
{
	return NodesNumber;
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::insert(T* data, int grade)
{
	return insertAux(this->_root, data, grade, 0, 0);
}

template<class T>
inline TreeNode<T>* RankedAVL<T>::remove(T* data, int grade)
{
	return removeAux(this->_root, data, grade);
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
inline void RankedAVL<T>::sumBumpGrade(int m, int* sum)
{
	sumBumpGradeAux(_root, m, sum);
}

template<class T>
inline RankedAVL<T>::RankedAVL(const RankedAVL<T>& rankedTree)
{
	_root = copyNodes(rankedTree._root);
	this->NodesNumber = rankedTree.NodesNumber;
}

template<class T>
inline void RankedAVL<T>::deleteNodes(TreeNode<T>* root)
{
	deleteTree(this->_root);
}

#endif // RANKED_AVL_TREE_H_