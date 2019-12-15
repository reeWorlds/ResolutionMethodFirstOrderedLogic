#pragma once

#include "Formula.h"
#include "Disjunction.h"
#include "Conjunction.h"
#include "Negation.h"
#include "Implacation.h"
#include "Predicate.h"
#include "Parser.h"

#include "FormulaTransformator.h"
#include "FormulaConverter.h"

#include <vector>
#include <set>
#include <QString>

using namespace std;


class RMImplementer
{
private:

	static vector<QString> getConstants(vector<Disjunct>& data);
	static vector<QString> getVariables(Disjunct& disjunct);
	
	static void copyDisjunct(Disjunct& a, Disjunct& sample);
	static void replaceVariable(Disjunct& a, QString& oldName, QString& newName);
	static void makeConstantDisjunctsRecursion(set<Disjunct>& result, vector<QString>& constants, vector<QString>& variables,
		vector<QString> namesInfo, Disjunct& baseDisjunct, int position);


	static int contraPairCount(Disjunct& a, Disjunct& b);
	static Disjunct getResolvente(Disjunct& a, Disjunct& b);

public:

	static vector<Disjunct> makeConstantDisjuncts(vector<Disjunct>& data);


	static QString tryToProve(vector<Disjunct>& data);

};