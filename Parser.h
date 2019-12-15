#pragma once

#include <vector>
#include <QString>

#include "Formula.h"
#include "Disjunction.h"
#include "Conjunction.h"
#include "Negation.h"
#include "Implacation.h"
#include "Predicate.h"

using namespace std;


class Parser
{
private:

	static int isPredicateSymbol(QChar symbol);
	static int isQuantifierSymbol(QChar symbol);
	static int isVariableSymbol(QChar symbol);
	static int isLogicalSymbol(QChar symbol);
	static int isCorrectSymbol(QChar symbol);

	static int quantifierHavePredicate(QString& input, int position);

	static int isCorrect0_1_2Formula(QString formula);


	static int getFormulaIndex(QString& formula, int start);
	static QString intToString(int number);

	static Formula* parseFormula(QString formula);

public:

	static int isCorrectFormula(QString input);

	static vector<Formula*> parseInput(QString input);

};