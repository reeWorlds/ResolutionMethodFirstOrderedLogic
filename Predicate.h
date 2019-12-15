#pragma once

#include "Formula.h"
#include "PredicateParameter.h"
#include "Variable.h"
#include "Function.h"

class Predicate : public Formula
{
public:

	Predicate();
	QString toString() const;

	bool operator <(const Predicate& other) const;

	QString name;
	vector<PredicateParameter*> parameters;
};