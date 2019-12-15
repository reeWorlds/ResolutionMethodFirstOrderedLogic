#pragma once

#include <QString>
#include <vector>
using namespace std;

enum PREDICATEPARAMETER
{
	variable,
	function
};

class PredicateParameter
{
public:

	virtual QString toString() = 0;

	PREDICATEPARAMETER type;

};