#include "Variable.h"

Variable::Variable()
{
	type = PREDICATEPARAMETER::variable;
}

QString Variable::toString()
{
	return name;
}