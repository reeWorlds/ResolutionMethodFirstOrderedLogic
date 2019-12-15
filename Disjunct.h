#pragma once

#include <QString>
#include <set>

#include "Variable.h"
#include "Function.h"

#include "Formula.h"
#include "Disjunction.h"
#include "Conjunction.h"
#include "Negation.h"
#include "Implacation.h"
#include "Predicate.h"

using namespace std;

class Disjunct
{
public:

	set<Predicate> truePredicates;
	set<Predicate> falsePredicates;

	void resetPredacates();

	bool operator <(const Disjunct& other) const;
	bool operator ==(const Disjunct& other) const;

	int haveContraPair();
	int isEmpty();
	QString toString() const;

};