#pragma once

#include "Formula.h"

#define disjunctionSymbol 0x2228

class Disjunction : public Formula
{
public:

	Disjunction();

	vector<Formula*> children;

};