#pragma once

#include "Formula.h"

#define implicationSymbol 0x2192

class Implication : public Formula
{
public:

	Implication();

	Formula* child1;
	Formula* child2;

};