#ifndef COMPANIES_UF_H_
#define COMAPNIES_UF_H_

#include <iostream>
#include "inverted_tree.h"
#include "company.h"

/*
	array of conpanies, in every index of the array we put an invert tree
*/

class CompaniesUF {
private:
	InvertedTree<Company*>** companies; 

public:

};

#endif //COMPANIES_UF_H