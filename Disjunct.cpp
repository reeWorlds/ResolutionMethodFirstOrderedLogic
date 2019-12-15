#include "Disjunct.h"

bool Disjunct::operator <(const Disjunct& other) const
{
	return toString() < other.toString();
}

bool Disjunct::operator ==(const Disjunct& other) const
{
	return toString() == other.toString();
}

void Disjunct::resetPredacates()
{
	set<Predicate> true_, false_;

	for (auto& it : truePredicates)
	{
		true_.insert(it);
	}

	for (auto& it : falsePredicates)
	{
		false_.insert(it);
	}

	truePredicates = true_;
	falsePredicates = false_;
}

int Disjunct::haveContraPair()
{
	for (auto& it : truePredicates)
	{
		if (falsePredicates.find(it) != falsePredicates.end())
		{
			return 1;
		}
	}

	return 0;
}

QString Disjunct::toString() const
{
	if (truePredicates.empty() && falsePredicates.empty())
	{
		return QString::fromStdWString(L"Порожній диз'юнкт");
	}

	QString result;

	for (auto it : truePredicates)
	{
		result += it.toString();
		result += QChar(disjunctionSymbol);
	}
	for (auto it : falsePredicates)
	{
		result += QChar(negationSymbol);
		result += it.toString();
		result += QChar(disjunctionSymbol);
	}

	result.chop(1);

	return result;
}

int Disjunct::isEmpty()
{
	if (truePredicates.empty() && falsePredicates.empty())
	{
		return 1;
	}

	return 0;
}