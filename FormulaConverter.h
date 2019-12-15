#pragma once

#include <vector>
#include <QString>

#include "Formula.h"
#include "Disjunction.h"
#include "Conjunction.h"
#include "Negation.h"
#include "Implacation.h"
#include "Predicate.h"

class FormulaConverter
{
private:

	static void formulaToStringRecursion(QString& result, Formula* formula);

public:

	static QString formulaToString(Formula* formula);

};