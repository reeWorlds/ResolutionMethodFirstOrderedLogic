#pragma once

#include "PredicateParameter.h"

class Variable : public PredicateParameter
{
public:

	Variable();

	virtual QString toString();

	QString name;
};