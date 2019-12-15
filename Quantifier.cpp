#include "Quantifier.h"

Quantifier::Quantifier()
{

}

Quantifier::Quantifier(QUANTIFIER type_, QString variableName_) : type(type_), variableName(variableName_)
{

}

void Quantifier::inverse()
{
	if (type == QUANTIFIER::exist)
	{
		type = QUANTIFIER::forany;
	}
	else
	{
		type = QUANTIFIER::exist;
	}
}