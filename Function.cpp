#include "Function.h"

Function::Function()
{
	type = PREDICATEPARAMETER::function;
}

QString Function::toString()
{
	QString result;

	result += name;
	result += "(";
	for (auto it : parameters)
	{
		result += it.name;
		result += ",";
	}
	if (result.back() == ',')
	{
		result.chop(1);
	}
	result += ")";

	return result;
}