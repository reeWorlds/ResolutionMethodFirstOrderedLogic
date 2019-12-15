#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "Quantifier.h"

using namespace std;

enum FORMULA
{
	predicate,
	negation,
	conjunction,
	disjunction,
	implication
};

class Formula
{
public:

	FORMULA type;
	vector<Quantifier> quantifiers;
};

