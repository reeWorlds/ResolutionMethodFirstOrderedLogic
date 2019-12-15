#include "Predicate.h"

Predicate::Predicate()
{
	type = FORMULA::predicate;
}

QString Predicate::toString() const
{
	QString result;

	result += name;
	result += "(";
	for (auto& it : parameters)
	{
		result += it->toString();
		result += ",";
	}
	if (result.back() == ',')
	{
		result.chop(1);
	}
	result += ")";

	return result;
}

bool Predicate::operator <(const Predicate& other) const
{
	return  (toString() < other.toString());
}