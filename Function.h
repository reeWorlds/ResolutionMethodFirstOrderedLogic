#pragma once

#include "PredicateParameter.h"
#include "Variable.h"

class Function : public PredicateParameter
{
public:

	Function();

	virtual QString toString();

	QString name;
	vector<Variable> parameters;

};