#pragma once

#include "Formula.h"

#define conjunctionSymbol 0x2227

class Conjunction : public Formula
{
public:

	Conjunction();

	vector<Formula*> children;
};