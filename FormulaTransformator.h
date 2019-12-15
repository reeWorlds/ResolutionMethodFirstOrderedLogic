#pragma once

#include <vector>
#include <QString>
#include <map>

#include "Formula.h"
#include "Disjunction.h"
#include "Conjunction.h"
#include "Negation.h"
#include "Implacation.h"
#include "Predicate.h"

#include "Disjunct.h"

using namespace std;

class FormulaTransformator
{
private:
	static void changeAllVariables(Formula* formula, QString oldName, QString newName);
	static void changeAllVariableOnFunction(Formula* formula, QString oldName, Function newFunction);


	static Formula* pushNegation(Formula* formula);
	static Formula* transformFormula(Formula* formula);

	static void disToConRecursion(vector<Formula*>& data, int pos, Disjunction* tempResult, vector<Formula*>& result);
	static vector<Formula*> disjunctionToConjunction(Disjunction* formula);

	static void conToDisRecursion(vector<Formula*>& data, int pos, Conjunction* tempResult, vector<Formula*>& result);
	static vector<Formula*> conjunctionToDisjunction(Conjunction* formula);

	static Disjunct formulaToDisjunct(Formula* formula);

	static Conjunction* copyConjunction(Conjunction* formula);
	static Disjunction* copyDisjunction(Disjunction* formula);

public:

	static Formula* replaceImplication(Formula*);

	static void unifyVariables(map<QChar, int>& variablesData, Formula*& formula);

	static void pushQuantifiers(Formula*& formula);

	static void removeQuantifiers(Formula*& formula, int& functionCount, int& variableCount);


	static vector<Disjunct> simplifyFormula(Formula* root);

};