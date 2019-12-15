#include "FormulaTransformator.h"

Formula* FormulaTransformator::replaceImplication(Formula* formula)
{
	if (formula->type == FORMULA::predicate)
	{
		Predicate* tempFormula = new Predicate;

		tempFormula->name = ((Predicate*)formula)->name;
		for (auto& it : ((Predicate*)formula)->parameters)
		{
			tempFormula->parameters.push_back(it);
		}
		for (auto& it : formula->quantifiers)
		{
			tempFormula->quantifiers.push_back(it);
		}

		return tempFormula;
	}
	else if (formula->type == FORMULA::negation)
	{
		Negation* tempFormula = new Negation;

		for (auto& it : formula->quantifiers)
		{
			tempFormula->quantifiers.push_back(it);
		}

		tempFormula->child = replaceImplication(((Negation*)formula)->child);

		return tempFormula;
	}
	else if (formula->type == FORMULA::conjunction)
	{
		Conjunction* tempFormula = new Conjunction;

		for (auto& it : formula->quantifiers)
		{
			tempFormula->quantifiers.push_back(it);
		}

		for (auto& it : ((Conjunction*)formula)->children)
		{
			tempFormula->children.push_back(replaceImplication(it));
		}

		return tempFormula;
	}
	else if (formula->type == FORMULA::disjunction)
	{
		Disjunction* tempFormula = new Disjunction;

		for (auto& it : formula->quantifiers)
		{
			tempFormula->quantifiers.push_back(it);
		}

		for (auto& it : ((Disjunction*)formula)->children)
		{
			tempFormula->children.push_back(replaceImplication(it));
		}

		return tempFormula;
	}
	else if (formula->type == FORMULA::implication)
	{
		Disjunction* tempFormula = new Disjunction;
		Negation* negationTempFormula = new Negation;

		for (auto& it : formula->quantifiers)
		{
			tempFormula->quantifiers.push_back(it);
		}

		negationTempFormula->child = replaceImplication(((Implication*)formula)->child1);
		tempFormula->children.push_back(negationTempFormula);

		tempFormula->children.push_back(replaceImplication(((Implication*)formula)->child2));

		return tempFormula;
	}
}

void FormulaTransformator::changeAllVariables(Formula* formula, QString oldVariable, QString newVariable)
{
	for (auto& it : formula->quantifiers)
	{
		if (it.variableName == oldVariable)
		{
			it.variableName = newVariable;
		}
	}

	if (formula->type == FORMULA::predicate)
	{
		for (auto& it : ((Predicate*)formula)->parameters)
		{
			if (it->type == PREDICATEPARAMETER::variable)
			{
				Variable* tempVariable = (Variable*)it;

				if (tempVariable->name == oldVariable)
				{
					tempVariable->name = newVariable;
				}
			}
			else
			{
				Function* tempFunction = (Function*)it;

				for (auto& itt : tempFunction->parameters)
				{
					if (itt.name == oldVariable)
					{
						itt.name = newVariable;
					}
				}
			}
		}
	}
	else if (formula->type == FORMULA::negation)
	{
		changeAllVariables(((Negation*)formula)->child, oldVariable, newVariable);
	}
	else if (formula->type == FORMULA::conjunction)
	{
		for (auto& it : ((Conjunction*)formula)->children)
		{
			changeAllVariables(it, oldVariable, newVariable);
		}
	}
	else if (formula->type == FORMULA::disjunction)
	{
		for (auto& it : ((Disjunction*)formula)->children)
		{
			changeAllVariables(it, oldVariable, newVariable);
		}
	}
	else if (formula->type == FORMULA::implication)
	{
		assert("WTF in FormulaTransformation::changeAllVariables find implication" == nullptr);
	}
}

void FormulaTransformator::changeAllVariableOnFunction(Formula* formula, QString oldVariable, Function newFunction)
{
	if (formula->type == FORMULA::predicate)
	{
		for (auto& it : ((Predicate*)formula)->parameters)
		{
			if (it->type == PREDICATEPARAMETER::variable)
			{
				Variable* tempVariable = (Variable*)it;

				if (tempVariable->name == oldVariable)
				{
					delete it;

					it = new Function;

					*((Function*)it) = newFunction;
				}
			}
			else
			{
				continue;
			}
		}
	}
	else if (formula->type == FORMULA::negation)
	{
		changeAllVariableOnFunction(((Negation*)formula)->child, oldVariable, newFunction);
	}
	else if (formula->type == FORMULA::conjunction)
	{
		for (auto& it : ((Conjunction*)formula)->children)
		{
			changeAllVariableOnFunction(it, oldVariable, newFunction);
		}
	}
	else if (formula->type == FORMULA::disjunction)
	{
		for (auto& it : ((Disjunction*)formula)->children)
		{
			changeAllVariableOnFunction(it, oldVariable, newFunction);
		}
	}
	else if (formula->type == FORMULA::implication)
	{
		assert("WTF in FormulaTransformation::changeAllVariablesOnFunction find implication" == nullptr);
	}
}

void FormulaTransformator::unifyVariables(map<QChar, int>& variablesData, Formula*& formula)
{
	for (auto& it: formula->quantifiers)
	{
		QString oldName = it.variableName;
		
		variablesData[it.variableName[0]]++;
		QString newName = it.variableName[0];
		newName += QString::number(variablesData[it.variableName[0]]);

		changeAllVariables(formula, oldName, newName);
	}

	// go to children

	if (formula->type == FORMULA::predicate)
	{
		return;
	}
	else if (formula->type == FORMULA::negation)
	{
		unifyVariables(variablesData, ((Negation*)formula)->child);
	}
	else if (formula->type == FORMULA::conjunction)
	{
		for (auto& it : ((Conjunction*)formula)->children)
		{
			unifyVariables(variablesData, it);
		}
	}
	else if (formula->type == FORMULA::disjunction)
	{
		for (auto& it : ((Disjunction*)formula)->children)
		{
			unifyVariables(variablesData, it);
		}
	}
	else if (formula->type == FORMULA::implication)
	{
		assert("WTF in FormulaTransformation::unifyVariables find implication" == nullptr);
	}
}

void FormulaTransformator::pushQuantifiers(Formula*& formula)
{
	if (formula->type == FORMULA::predicate)
	{
		return;
	}
	else if (formula->type == FORMULA::negation)
	{ 
		pushQuantifiers(((Negation*)formula)->child);
		
		for (auto& it : ((Negation*)formula)->child->quantifiers)
		{
			formula->quantifiers.push_back(it);
			formula->quantifiers.back().inverse();
		}
		((Negation*)formula)->child->quantifiers.clear();
	}
	else if (formula->type == FORMULA::conjunction)
	{
		assert(((Conjunction*)formula)->children.size() == 2);

		for (auto& it : ((Conjunction*)formula)->children)
		{
			pushQuantifiers(it);
		}

		Formula* child1 = ((Conjunction*)formula)->children[0];
		Formula* child2 = ((Conjunction*)formula)->children[1];

		while (!child1->quantifiers.empty() && child1->quantifiers[0].type == QUANTIFIER::exist)
		{
			formula->quantifiers.push_back(child1->quantifiers[0]);
			child1->quantifiers.erase(child1->quantifiers.begin());
		}
		while (!child2->quantifiers.empty() && child2->quantifiers[0].type == QUANTIFIER::exist)
		{
			formula->quantifiers.push_back(child2->quantifiers[0]);
			child2->quantifiers.erase(child2->quantifiers.begin());
		}

		for (auto& it : child1->quantifiers)
		{
			formula->quantifiers.push_back(it);
		}
		for (auto& it : child2->quantifiers)
		{
			formula->quantifiers.push_back(it);
		}
		
		child1->quantifiers.clear();
		child2->quantifiers.clear();
	}
	else if (formula->type == FORMULA::disjunction)
	{
		assert(((Disjunction*)formula)->children.size() == 2);

		for (auto& it : ((Disjunction*)formula)->children)
		{
			pushQuantifiers(it);
		}

		Formula* child1 = ((Disjunction*)formula)->children[0];
		Formula* child2 = ((Disjunction*)formula)->children[1];

		while (!child1->quantifiers.empty() && child1->quantifiers[0].type == QUANTIFIER::exist)
		{
			formula->quantifiers.push_back(child1->quantifiers[0]);
			child1->quantifiers.erase(child1->quantifiers.begin());
		}
		while (!child2->quantifiers.empty() && child2->quantifiers[0].type == QUANTIFIER::exist)
		{
			formula->quantifiers.push_back(child2->quantifiers[0]);
			child2->quantifiers.erase(child2->quantifiers.begin());
		}

		for (auto& it : child1->quantifiers)
		{
			formula->quantifiers.push_back(it);
		}
		for (auto& it : child2->quantifiers)
		{
			formula->quantifiers.push_back(it);
		}

		child1->quantifiers.clear();
		child2->quantifiers.clear();
	}
	else if(formula->type == FORMULA::implication)
	{
		assert("WTF in FormulaTransformation::pushQuantifiers find implication" == nullptr);
	}
}

void FormulaTransformator::removeQuantifiers(Formula*& formula, int& functionCount, int& variableCount)
{
	vector<Variable> foranyVariables;

	for (auto& it : formula->quantifiers)
	{
		if (it.type == QUANTIFIER::forany)
		{
			foranyVariables.push_back(Variable());
			foranyVariables.back().name = it.variableName;
		}
		else
		{
			if (foranyVariables.empty())
			{
				variableCount++;
				QString newVariableName = "a" + QString::number(variableCount);

				changeAllVariables(formula, it.variableName, newVariableName);
			}
			else
			{
				functionCount++;
				Function newFunction;
				newFunction.name = "f" + QString::number(functionCount);
				newFunction.parameters = foranyVariables;

				changeAllVariableOnFunction(formula, it.variableName, newFunction);
			}
		}
	}

	formula->quantifiers.clear();
}

Formula* FormulaTransformator::pushNegation(Formula* formula)
{
	if (formula->type == FORMULA::predicate)
	{
		Predicate* temp = new Predicate;
		temp->name = ((Predicate*)formula)->name;
		for (auto& it : ((Predicate*)formula)->parameters)
		{
			temp->parameters.push_back(it);
		}

		return temp;
	}
	else if (formula->type == FORMULA::conjunction)
	{
		Conjunction* temp = new Conjunction;

		for (auto& it : ((Conjunction*)formula)->children)
		{
			temp->children.push_back(pushNegation(it));
		}

		return temp;
	}
	else if (formula->type == FORMULA::disjunction)
	{
		Disjunction* temp = new Disjunction;

		for (auto& it : ((Disjunction*)formula)->children)
		{
			temp->children.push_back(pushNegation(it));
		}

		return temp;
	}
	else if (formula->type == FORMULA::negation)
	{
		Negation* info = (Negation*)formula;

		if (info->child->type == FORMULA::predicate)
		{
			return info;
		}
		else if (info->child->type == FORMULA::negation)
		{
			return pushNegation(((Negation*)(info->child))->child);
		}
		else if (info->child->type == FORMULA::conjunction)
		{
			Disjunction* temp = new Disjunction();
			for (auto& it : ((Conjunction*)(info->child))->children)
			{
				Negation* tempNegation = new Negation;
				tempNegation->child = it;

				temp->children.push_back(pushNegation(tempNegation));
			}

			return temp;
		}
		else if (info->child->type == FORMULA::disjunction)
		{
			Conjunction* temp = new Conjunction();
			for (auto& it : ((Disjunction*)(info->child))->children)
			{
				Negation* tempNegation = new Negation;
				tempNegation->child = it;

				temp->children.push_back(pushNegation(tempNegation));
			}

			return temp;
		}
	}
}

Conjunction* FormulaTransformator::copyConjunction(Conjunction* formula)
{
	Conjunction* newFormula = new Conjunction;

	for (auto& it : formula->children)
	{
		newFormula->children.push_back(it);
	}

	return newFormula;
}

Disjunction* FormulaTransformator::copyDisjunction(Disjunction* formula)
{
	Disjunction* newFormula = new Disjunction;

	for (auto& it : formula->children)
	{
		newFormula->children.push_back(it);
	}

	return newFormula;
}

void FormulaTransformator::disToConRecursion(std::vector<Formula*>& data, int pos, Disjunction* tempResult, std::vector<Formula*>& result)
{
	if (pos == data.size())
	{
		result.push_back(copyDisjunction(tempResult));

		return;
	}

	if (data[pos]->type == FORMULA::predicate || data[pos]->type == FORMULA::negation)
	{
		tempResult->children.push_back(data[pos]);
		disToConRecursion(data, pos + 1, tempResult, result);
		tempResult->children.pop_back();

		return;
	}
	else if (data[pos]->type == FORMULA::conjunction) // i am sure it is conjunction anyway (!) [i hope]
	{
		for (auto& it : (((Conjunction*)(data[pos]))->children))
		{
			tempResult->children.push_back(it);
			disToConRecursion(data, pos + 1, tempResult, result);
			tempResult->children.pop_back();
		}

		return;
	}
}

std::vector<Formula*> FormulaTransformator::disjunctionToConjunction(Disjunction* formula)
{
	std::vector<Formula*> result;
	Disjunction* tempResult = new Disjunction;

	disToConRecursion(formula->children, 0, tempResult, result);

	return result;
}

void FormulaTransformator::conToDisRecursion(std::vector<Formula*>& data, int pos, Conjunction* tempResult, std::vector<Formula*>& result)
{
	if (pos == data.size())
	{
		result.push_back(copyConjunction(tempResult));

		return;
	}

	if (data[pos]->type == FORMULA::predicate || data[pos]->type == FORMULA::negation)
	{
		tempResult->children.push_back(data[pos]);
		conToDisRecursion(data, pos + 1, tempResult, result);
		tempResult->children.pop_back();

		return;
	}
	else if (data[pos]->type == FORMULA::disjunction) // i am sure it is disjunction anyway (!) [i hope]
	{
		for (auto& it : (((Disjunction*)(data[pos]))->children))
		{
			tempResult->children.push_back(it);
			conToDisRecursion(data, pos + 1, tempResult, result);
			tempResult->children.pop_back();
		}

		return;
	}
}

vector<Formula*> FormulaTransformator::conjunctionToDisjunction(Conjunction* formula)
{
	std::vector<Formula*> result;
	Conjunction* tempResult = new Conjunction;

	conToDisRecursion(formula->children, 0, tempResult, result);

	return result;
}

Formula* FormulaTransformator::transformFormula(Formula* formula)
{
	// reduce deep of conjunctions and disjunctions
	// be sure to push negation before

	if (formula->type == FORMULA::predicate)
	{
		return formula;
	}
	else if (formula->type == FORMULA::negation)
	{
		return formula;
	}
	else if (formula->type == FORMULA::conjunction)
	{
		Conjunction* result = new Conjunction;

		for (auto& it : ((Conjunction*)formula)->children)
		{
			Formula* transformedFormula = transformFormula(it);

			if (transformedFormula->type == FORMULA::predicate || transformedFormula->type == FORMULA::negation)
			{
				result->children.push_back(transformedFormula);
			}
			else if (transformedFormula->type == FORMULA::conjunction)
			{
				for (auto& itt : ((Conjunction*)transformedFormula)->children)
				{
					result->children.push_back(itt);
				}
			}
			else if (transformedFormula->type == FORMULA::disjunction)
			{
				std::vector<Formula*> conjunctions = disjunctionToConjunction((Disjunction*)transformedFormula);

				for (auto& itt : conjunctions)
				{
					result->children.push_back(itt);
				}
			}
		}

		return result;
	}
	else if (formula->type == FORMULA::disjunction)
	{
		Disjunction* result = new Disjunction;

		for (auto& it : ((Disjunction*)formula)->children)
		{
			Formula* transformedFormula = transformFormula(it);

			if (transformedFormula->type == FORMULA::predicate || transformedFormula->type == FORMULA::negation)
			{
				result->children.push_back(transformedFormula);
			}
			else if (transformedFormula->type == FORMULA::disjunction)
			{
				for (auto& itt : ((Disjunction*)transformedFormula)->children)
				{
					result->children.push_back(itt);
				}
			}
			else if (transformedFormula->type == FORMULA::conjunction)
			{
				std::vector<Formula*> disjunctions = conjunctionToDisjunction((Conjunction*)transformedFormula);

				for (auto& itt : disjunctions)
				{
					result->children.push_back(itt);
				}
			}
		}

		return result;
	}
}

Disjunct FormulaTransformator::formulaToDisjunct(Formula* formula)
{
	// I hope there is not any conjunction there

	Disjunct result;

	if (formula->type == FORMULA::predicate)
	{
		result.truePredicates.insert(*((Predicate*)formula));
		//result.data1[((Predicate*)formula)->name - 'A'] = 1;
	}
	else if (formula->type == FORMULA::negation)
	{
		result.falsePredicates.insert(*((Predicate*)(((Negation*)formula)->child)));
		//result.data0[((Predicate*)(((Negation*)formula)->child))->name - 'A'] = 1;
	}
	else if (formula->type == FORMULA::disjunction)
	{
		for (auto& it : ((Disjunction*)formula)->children)
		{
			if (it->type == FORMULA::predicate)
			{
				result.truePredicates.insert(*((Predicate*)it));
			}
			else if (it->type == FORMULA::negation)
			{
				result.falsePredicates.insert(*((Predicate*)(((Negation*)it)->child)));
			}
		}
	}

	return result;
}

vector<Disjunct> FormulaTransformator::simplifyFormula(Formula* root)
{
	Formula* root1 = pushNegation(root);

	if (root1->type == FORMULA::predicate)
	{
		vector<Disjunct> result;
		result.push_back(Disjunct());
		result[0].truePredicates.insert(*((Predicate*)root1));
		//result[0].data1[((Predicate*)root1)->name - 'A'] = 1;

		return result;
	}
	else if (root1->type == FORMULA::negation)
	{
		std::vector<Disjunct> result;
		result.push_back(Disjunct());
		result[0].falsePredicates.insert(*((Predicate*)(((Negation*)root1)->child)));
		//result[0].data0[((Predicate*)(((Negation*)root1)->child))->name - 'A'] = 1;

		return result;
	}
	else
	{
		Formula* root2 = transformFormula(root1);

		if (root2->type == FORMULA::disjunction)
		{
			std::vector<Formula*> root3 = disjunctionToConjunction((Disjunction*)root2);

			std::vector<Disjunct> result;
			Disjunct tempDisjunct;

			for (auto& it : root3)
			{
				tempDisjunct = formulaToDisjunct(it);

				if (!tempDisjunct.haveContraPair())
				{
					result.push_back(tempDisjunct);
				}
			}

			return result;
		}
		else
		{
			std::vector<Disjunct> result;
			Disjunct tempDisjunct;

			for (auto& it : ((Conjunction*)root2)->children)
			{
				tempDisjunct = formulaToDisjunct(it);

				if (!tempDisjunct.haveContraPair())
				{
					result.push_back(tempDisjunct);
				}
			}

			return result;
		}
	}
}

