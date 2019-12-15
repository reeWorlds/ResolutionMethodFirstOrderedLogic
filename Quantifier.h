#pragma once

#include <QString>
#include <vector>
using namespace std;

enum QUANTIFIER
{
	exist,
	forany
};

#define existSymbol 0x2203
#define foranySymbol 0x2200

class Quantifier
{
public:

	Quantifier();
	Quantifier(QUANTIFIER type_, QString variableName_);

	void inverse();
	
	QUANTIFIER type;
	QString variableName;
};