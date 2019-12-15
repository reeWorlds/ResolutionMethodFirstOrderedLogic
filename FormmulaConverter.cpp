#include "FormulaConverter.h"

void FormulaConverter::formulaToStringRecursion(QString& result, Formula* formula)
{
	for (auto& it : formula->quantifiers)
	{
		if (it.type == QUANTIFIER::exist)
		{
			result += QChar(existSymbol);
		}
		else
		{
			result += QChar(foranySymbol);
		}
		result += it.variableName;
	}

	if (formula->type == FORMULA::predicate)
	{
		Predicate* tempPredicate = (Predicate*)formula;

		result += tempPredicate->name;

		result += "(";
		for (auto& it : tempPredicate->parameters)
		{
			result += it->toString();
			result += ",";
		}
		if (result.back() == ',')
		{
			result.chop(1);
		}
		result += ")";
	}
	else if (formula->type == FORMULA::negation)
	{
		result += QChar(negationSymbol);

		if (((Negation*)formula)->child->type != FORMULA::predicate)
		{
			result += "(";
		}
		
		formulaToStringRecursion(result, ((Negation*)formula)->child);

		if (((Negation*)formula)->child->type != FORMULA::predicate)
		{
			result += ")";
		}
	}
	else if (formula->type == FORMULA::conjunction)
	{
		Conjunction* tempConjunction = (Conjunction*)formula;

		if (!formula->quantifiers.empty())
		{
			result += "(";
		}

		for (auto& it : tempConjunction->children)
		{
			if (it->type != FORMULA::predicate)
			{
				result += "(";
			}
			formulaToStringRecursion(result, it);
			if (it->type != FORMULA::predicate)
			{
				result += ")";
			}
			result += QChar(conjunctionSymbol);
		}
		if (result.back() == QChar(conjunctionSymbol))
		{
			result.chop(1);
		}

		if (!formula->quantifiers.empty())
		{
			result += ")";
		}
	}
	else if (formula->type == FORMULA::disjunction)
	{
		Disjunction* tempDisjunction = (Disjunction*)formula;

		if (!formula->quantifiers.empty())
		{
			result += "(";
		}

		for (auto& it : tempDisjunction->children)
		{
			if (it->type != FORMULA::predicate)
			{
				result += "(";
			}
			formulaToStringRecursion(result, it);
			if (it->type != FORMULA::predicate)
			{
				result += ")";
			}
			result += QChar(disjunctionSymbol);
		}
		if (result.back() == QChar(disjunctionSymbol))
		{
			result.chop(1);
		}

		if (!formula->quantifiers.empty())
		{
			result += ")";
		}
	}
	else if (formula->type == FORMULA::implication)
	{
		Implication* tempImplication = (Implication*)formula;

		if (!formula->quantifiers.empty())
		{
			result += "(";
		}

		if (tempImplication->child1->type != FORMULA::predicate)
		{
			result += "(";
		}
		formulaToStringRecursion(result, tempImplication->child1);
		if (tempImplication->child1->type != FORMULA::predicate)
		{
			result += ")";
		}

		result += QChar(implicationSymbol);

		if (tempImplication->child2->type != FORMULA::predicate)
		{
			result += "(";
		}
		formulaToStringRecursion(result, tempImplication->child2);
		if (tempImplication->child2->type != FORMULA::predicate)
		{
			result += ")";
		}

		if (!formula->quantifiers.empty())
		{
			result += ")";
		}
	}
}

QString FormulaConverter::formulaToString(Formula* formula)
{
	QString result;

	formulaToStringRecursion(result, formula);
	
	return result;
}
