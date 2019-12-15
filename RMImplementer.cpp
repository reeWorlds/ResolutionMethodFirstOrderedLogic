#include "RMImplementer.h"

vector<QString> RMImplementer::getConstants(vector<Disjunct>& data)
{
	set<QString> resultSet;

	for (auto& disjunct : data)
	{
		for (auto& predicate : disjunct.truePredicates)
		{
			for (auto& predicateParameter : predicate.parameters)
			{
				if (predicateParameter->type == PREDICATEPARAMETER::variable)
				{
					if (((Variable*)predicateParameter)->name[0] == 'a')
					{
						resultSet.insert(((Variable*)predicateParameter)->name);
					}
				}
				else
				{
					for (auto& variable : ((Function*)predicateParameter)->parameters)
					{
						if (variable.name[0] == 'a')
						{
							resultSet.insert(variable.name);
						}
					}
				}
			}
		}
		for (auto& predicate : disjunct.falsePredicates)
		{
			for (auto& predicateParameter : predicate.parameters)
			{
				if (predicateParameter->type == PREDICATEPARAMETER::variable)
				{
					if (((Variable*)predicateParameter)->name[0] == 'a')
					{
						resultSet.insert(((Variable*)predicateParameter)->name);
					}
				}
				else
				{
					for (auto& variable : ((Function*)predicateParameter)->parameters)
					{
						if (variable.name[0] == 'a')
						{
							resultSet.insert(variable.name);
						}
					}
				}
			}
		}
	}

	vector<QString> result;
	for (auto& it : resultSet)
	{
		result.push_back(it);
	}

	return result;
}

vector<QString> RMImplementer::getVariables(Disjunct& disjunct)
{
	set<QString> resultSet;

	for (auto& predicate : disjunct.truePredicates)
	{
		for (auto& predicateParameter : predicate.parameters)
		{
			if (predicateParameter->type == PREDICATEPARAMETER::variable)
			{
				if (((Variable*)predicateParameter)->name[0] != 'a')
				{
					resultSet.insert(((Variable*)predicateParameter)->name);
				}
			}
			else
			{
				for (auto& variable : ((Function*)predicateParameter)->parameters)
				{
					if (variable.name[0] != 'a')
					{
						resultSet.insert(variable.name);
					}
				}
			}
		}
	}
	for (auto& predicate : disjunct.falsePredicates)
	{
		for (auto& predicateParameter : predicate.parameters)
		{
			if (predicateParameter->type == PREDICATEPARAMETER::variable)
			{
				if (((Variable*)predicateParameter)->name[0] != 'a')
				{
					resultSet.insert(((Variable*)predicateParameter)->name);
				}
			}
			else
			{
				for (auto& variable : ((Function*)predicateParameter)->parameters)
				{
					if (variable.name[0] != 'a')
					{
						resultSet.insert(variable.name);
					}
				}
			}
		}
	}

	vector<QString> result;
	for (auto& it : resultSet)
	{
		result.push_back(it);
	}

	return result;
}

void RMImplementer::copyDisjunct(Disjunct& a, Disjunct& sample)
{
	for (auto& predicate : sample.truePredicates)
	{
		Predicate tempPredicate;

		tempPredicate.name = predicate.name;
		for (auto& parameter : predicate.parameters)
		{
			if (parameter->type == PREDICATEPARAMETER::variable)
			{
				tempPredicate.parameters.push_back(new Variable);
				((Variable*)tempPredicate.parameters.back())->name = ((Variable*)parameter)->name;
			}
			else
			{
				tempPredicate.parameters.push_back(new Function);
				((Function*)tempPredicate.parameters.back())->name = ((Function*)parameter)->name;
				for (auto& variable : ((Function*)parameter)->parameters)
				{
					((Function*)tempPredicate.parameters.back())->parameters.push_back(variable);
				}
			}
		}

		a.truePredicates.insert(tempPredicate);
	}

	for (auto& predicate : sample.falsePredicates)
	{
		Predicate tempPredicate;

		tempPredicate.name = predicate.name;
		for (auto& parameter : predicate.parameters)
		{
			if (parameter->type == PREDICATEPARAMETER::variable)
			{
				tempPredicate.parameters.push_back(new Variable);
				((Variable*)tempPredicate.parameters.back())->name = ((Variable*)parameter)->name;
			}
			else
			{
				tempPredicate.parameters.push_back(new Function);
				((Function*)tempPredicate.parameters.back())->name = ((Function*)parameter)->name;
				for (auto& variable : ((Function*)parameter)->parameters)
				{
					((Function*)tempPredicate.parameters.back())->parameters.push_back(variable);
				}
			}
		}

		a.falsePredicates.insert(tempPredicate);
	}
}

void RMImplementer::replaceVariable(Disjunct& a, QString& oldName, QString& newName)
{
	for (auto& predicate : a.truePredicates)
	{
		for (auto& parameter : predicate.parameters)
		{
			if (parameter->type == PREDICATEPARAMETER::variable)
			{
				if (((Variable*)parameter)->name == oldName)
				{
					((Variable*)parameter)->name = newName;
				}
			}
			else
			{
				for (auto& variable : ((Function*)parameter)->parameters)
				{
					if (variable.name == oldName)
					{
						variable.name = newName;
					}
				}
			}
		}
	}

	for (auto& predicate : a.falsePredicates)
	{
		for (auto& parameter : predicate.parameters)
		{
			if (parameter->type == PREDICATEPARAMETER::variable)
			{
				if (((Variable*)parameter)->name == oldName)
				{
					((Variable*)parameter)->name = newName;
				}
			}
			else
			{
				for (auto& variable : ((Function*)parameter)->parameters)
				{
					if (variable.name == oldName)
					{
						variable.name = newName;
					}
				}
			}
		}
	}
}

void RMImplementer::makeConstantDisjunctsRecursion(set<Disjunct>& result, vector<QString>& constants,
	vector<QString>& variables, vector<QString> namesInfo, Disjunct& baseDisjunct, int position)
{
	if (position == variables.size())
	{
		Disjunct tempDisjunct;
		copyDisjunct(tempDisjunct, baseDisjunct);
		for (int i = 0; i < variables.size(); i++)
		{
			replaceVariable(tempDisjunct, variables[i], namesInfo[i]);
		}

		tempDisjunct.resetPredacates();

		if (!tempDisjunct.haveContraPair())
		{
			result.insert(tempDisjunct);
		}

		return;
	}

	for (int constantPosition = 0; constantPosition < constants.size(); constantPosition++)
	{
		namesInfo.push_back(constants[constantPosition]);
		makeConstantDisjunctsRecursion(result, constants, variables, namesInfo, baseDisjunct, position + 1);
		namesInfo.pop_back();
	}
}

vector<Disjunct> RMImplementer::makeConstantDisjuncts(vector<Disjunct>& data)
{
	set<Disjunct> result;

	vector<QString> constants = getConstants(data);
	if (constants.empty())
	{
		constants.push_back("a");
	}

	for (auto& it : data)
	{
		vector<QString> variables = getVariables(it);
		
		vector<QString> namesInfo;
		makeConstantDisjunctsRecursion(result, constants, variables, namesInfo, it, 0);
	}

	vector<Disjunct> vectorResult;
	for (auto& it : result)
	{
		vectorResult.push_back(it);
	}

	return vectorResult;
}

int RMImplementer::contraPairCount(Disjunct& a, Disjunct& b)
{
	int result = 0;

	for (auto& it : a.truePredicates)
	{
		if (b.falsePredicates.find(it) != b.falsePredicates.end())
		{
			result++;
		}
	}

	for (auto& it : a.falsePredicates)
	{
		if (b.truePredicates.find(it) != b.truePredicates.end())
		{
			result++;
		}
	}

	return result;
}

Disjunct RMImplementer::getResolvente(Disjunct& a, Disjunct& b)
{
	Disjunct result;

	for (auto& it : a.truePredicates)
	{
		result.truePredicates.insert(it);
	}
	for (auto& it : b.truePredicates)
	{
		result.truePredicates.insert(it);
	}

	for (auto& it : a.falsePredicates)
	{
		result.falsePredicates.insert(it);
	}
	for (auto& it : b.falsePredicates)
	{
		result.falsePredicates.insert(it);
	}

	vector<Predicate> repeats;
	for (auto& it : result.truePredicates)
	{
		if (result.falsePredicates.find(it) != result.falsePredicates.end())
		{
			repeats.push_back(it);
		}
	}

	assert(repeats.size() == 1);
	result.truePredicates.erase(repeats[0]);
	result.falsePredicates.erase(repeats[0]);

	return result;
}

QString RMImplementer::tryToProve(vector<Disjunct>& data)
{
	vector<Disjunct> info = data;
	vector<pair<int, int> > parent;

	set<QString> usedDisjuncts;
	for (auto& it : info)
	{
		parent.push_back({ -1, -1 });
		usedDisjuncts.insert(it.toString());
	}
	
	// to do this
	{
		for (auto& disjunct : info)
		{
			for (auto& predicate : disjunct.truePredicates)
			{
				for (auto& parameter : predicate.parameters)
				{
					if (parameter->type == PREDICATEPARAMETER::function)
					{
						return QString::fromStdWString(L"Виведення порожнього диз'юнкта не знайдено.\n\n");
					}
				}
			}
		}
	}


	for (int i = 0; i < info.size(); i++)
	{
		int lastIndex = info.size();

		for (int j = i + 1; j < lastIndex; j++)
		{
			if (contraPairCount(info[i], info[j]) == 1)
			{
				Disjunct newDisjunct = getResolvente(info[i], info[j]);

				if (usedDisjuncts.find(newDisjunct.toString()) != usedDisjuncts.end())
				{
					continue;
				}

				usedDisjuncts.insert(newDisjunct.toString());
				info.push_back(newDisjunct);
				parent.push_back({ i, j });

				if (info.back().isEmpty())
				{
					vector<int> used;
					used.resize(info.size());
					used.back() = 1;

					for (int usedI = used.size() - 1; usedI >= 0; usedI--)
					{
						if (used[usedI] == 1)
						{
							if (parent[usedI].first != -1)
							{
								used[parent[usedI].first] = 1;
								used[parent[usedI].second] = 1;
							}
						}
					}

					int proofNumberCount = 1;
					vector<int> proofNumber;
					proofNumber.resize(used.size());
					for (int proofNumberI = 0; proofNumberI < proofNumber.size(); proofNumberI++)
					{
						if (used[proofNumberI] == 1)
						{
							proofNumber[proofNumberI] = proofNumberCount;
							proofNumberCount++;
						}
						else
						{
							proofNumber[proofNumberI] = 0;
						}
					}

					QString result = QString::fromStdWString(L"Доведення:\n");
					for (int ii = 0; ii < used.size(); ii++)
					{
						if (used[ii] == 1)
						{
							result += QString::number(proofNumber[ii]) + ") ";
							result += info[ii].toString() + "  ";
							if (parent[ii].first == -1)
							{
								result += QString::fromStdWString(L"(Отримали з початкових диз'юнктів підстановкою)");
							}
							else
							{
								result += QString::fromStdWString(L"(Отримали як резольвенту з диз'юнктів з номерами ") +
									QString::number(proofNumber[parent[ii].first]) + QString::fromStdWString(L" і ") +
									QString::number(proofNumber[parent[ii].second]) + ")";
							}
							result += "\n";
						}
					}

					return result;
				}
			}
		}
	}

	return QString::fromStdWString(L"Виведення порожнього диз'юнкта не знайдено.\n\n");
}
