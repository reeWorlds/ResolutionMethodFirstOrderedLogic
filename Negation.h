#pragma once

#include "Formula.h"

#define negationSymbol 0x00AC

class Negation : public Formula
{
public:

	Negation();

	Formula* child;

};