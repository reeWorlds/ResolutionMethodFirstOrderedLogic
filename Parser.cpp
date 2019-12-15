#include "Parser.h"

int Parser::isPredicateSymbol(QChar symbol)
{
	if (symbol >= 'A' && symbol <= 'Z')
	{
		return 1;
	}

	return 0;
}

int Parser::isQuantifierSymbol(QChar symbol)
{
	if (symbol == QChar(existSymbol) || symbol == QChar(foranySymbol))
	{
		return 1;
	}

	return 0;
}

int Parser::isVariableSymbol(QChar symbol)
{
	if (symbol >= 'a' && symbol <= 'z')
	{
		return 1;
	}

	return 0;
}

int Parser::isLogicalSymbol(QChar symbol)
{
	if (symbol == '(' || symbol == ')' || symbol == ',' || symbol == '|' || symbol == '=' || symbol == QChar(disjunctionSymbol)
		|| symbol == QChar(conjunctionSymbol) || symbol == QChar(negationSymbol) || symbol == QChar(implicationSymbol))
	{
		return 1;
	}

	return 0;
}

int Parser::isCorrectSymbol(QChar symbol)
{
	if (isPredicateSymbol(symbol) || isQuantifierSymbol(symbol) || isVariableSymbol(symbol) || isLogicalSymbol(symbol))
	{
		return 1;
	}

	return 0;
}

int Parser::quantifierHavePredicate(QString& input, int position)
{
	for (int i = position + 1; i < int(input.size()); i++)
	{
		if (isQuantifierSymbol(input[i]) || isVariableSymbol(input[i]) || input[i] == '(' || input[i] == QChar(negationSymbol))
		{
			continue;
		}
		if (isPredicateSymbol(input[i]))
		{
			return 1;
		}
		return 0;
	}

	return 0;
}

int Parser::isCorrect0_1_2Formula(QString formula) // 0, 1, (, ), 2
{
	if (formula.isEmpty())
	{
		return 0;
	}

	if (int(formula.size()) == 1)
	{
		if (formula[0] == '1')
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int i, parenthesesCount = 0;

	for (i = 0; i + 1 < int(formula.size()); i++)
	{
		if (formula[i] == '0')
		{
			if (formula[i + 1] == '0' || formula[i + 1] == ')')
			{
				return 0;
			}
		}
		if (formula[i] == '1')
		{
			if (formula[i + 1] == '1' || formula[i + 1] == '(' || formula[i + 1] == '2')
			{
				return 0;
			}
		}
		if (formula[i] == '2')
		{
			if (formula[i + 1] == '0' || formula[i + 1] == ')')
			{
				return 0;
			}
		}
		if (formula[i] == '(')
		{
			if (formula[i + 1] == '0' || formula[i + 1] == ')')
			{
				return 0;
			}
		}
		if (formula[i] == ')')
		{
			if (formula[i + 1] == '1' || formula[i + 1] == '(' || formula[i + 1] == '2')
			{
				return 0;
			}
		}
	}

	if (formula[0] != '1' && formula[0] != '(' && formula[i] == '2')
	{
		return 0;
	}

	if (formula.back() != '1' && formula.back() != ')')
	{
		return 0;
	}

	for (i = 0; i<int(formula.size()); i++)
	{
		if (formula[i] == '(')
		{
			parenthesesCount++;
		}
		if (formula[i] == ')')
		{
			parenthesesCount--;

			if (parenthesesCount < 0)
			{
				return 0;
			}
		}
	}
	if (parenthesesCount != 0)
	{
		return 0;
	}

	return 1;
}

int Parser::isCorrectFormula(QString input)
{
	int i, j;

	// delete "white" symbols
	{
		QString tempFormula;

		for (i = 0; i < int(input.size()); i++)
		{
			if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
			{
				tempFormula += input[i];
			}
		}

		input = tempFormula;
	}

	if (input.isEmpty())
	{
		return 0;
	}

	// check for all symbols are correct
	for (i = 0; i< int(input.size()); i++)
	{
		if (!isCorrectSymbol(input[i]))
		{
			return 0;
		}
	}

	// delete all quatifiers
	for (i = 0; i<int(input.size()); i++)
	{
		if (isQuantifierSymbol(input[i]))
		{
			if (!quantifierHavePredicate(input, i) || (i + 1 == input.size()))
			{
				return 0;
			}

			if (!isVariableSymbol(input[i + 1]))
			{
				return 0;
			}

			QString tempString;
			
			for (j = 0; j < i; j++)
			{
				tempString += input[j];
			}
			for (j = i + 2; j < int(input.size()); j++)
			{
				tempString += input[j];
			}

			i--;
			input = tempString;
		}
	}

	// delete all parameters
	for (i = 0; i<int(input.size()); i++)
	{
		if (isPredicateSymbol(input[i]))
		{
			if (i + 1 == input.size())
			{
				continue;
			}
			if (input[i + 1] != '(')
			{
				continue;
			}

			int start_ = i + 1, end_ = i + 2;
			for (; end_<int(input.size()); end_++)
			{
				if (input[end_] == ')')
				{
					break;
				}
				if (input[end_] == ',' || isVariableSymbol(input[end_]))
				{
					continue;
				}

				return 0;
			}
			if (end_ == input.size())
			{
				return 0;
			}

			// check for correct parameters
			for (j = start_ + 1; j < end_; j++)
			{
				if ((j - start_) % 2 == 0)
				{
					if (input[j] != ',')
					{
						return 0;
					}
				}
				else
				{
					if (!isVariableSymbol(input[j]))
					{
						return 0;
					}
				}
			}

			// delete
			QString tempString;
			for (j = 0; j < start_; j++)
			{
				tempString += input[j];
			}
			for (j = end_ + 1; j < int(input.size()); j++)
			{
				tempString += input[j];
			}

			input = tempString;
		}
	}

	// make 0-1 string
	int countI = 0, countEqual = 0;

	for (i = 0; i<int(input.size()); i++)
	{
		if (input[i] == '|')
		{
			countI++;
		}
		if (input[i] == '=')
		{
			countEqual++;
		}
	}

	if (countEqual != 1 || countI != 1)
	{
		return 0;
	}

	{
		QString tempFormula;

		for (i = 0; i<int(input.size()); i++)
		{
			if (input[i] != '|' && input[i] != '=')
			{
				tempFormula += input[i];

				continue;
			}

			if (input[i] == '|')
			{
				if ((i + 1 == int(input.size())) || input[i + 1] != '=')
				{
					return 0;
				}

				if ((i + 2 != int(input.size())) && (i != 0))
				{
					tempFormula += ',';
				}
			}
		}

		input = tempFormula;
	}

	QString tempFormula;
	vector<QString> formulas;
	for (i = 0; i < int(input.size()); i++)
	{
		if (input[i] == ',')
		{
			formulas.push_back(tempFormula);
			tempFormula.clear();

			continue;
		}

		if (input[i] == '(' || input[i] == ')')
		{
			tempFormula += input[i];
		}
		else if (isPredicateSymbol(input[i]))
		{
			tempFormula += '1';
		}
		else if (input[i] == QChar(negationSymbol))
		{
			tempFormula += '2';
		}
		else if (input[i] == QChar(conjunctionSymbol) || input[i] == QChar(disjunctionSymbol) || input[i] == QChar(implicationSymbol))
		{
			tempFormula += '0';
		}
		else
		{
			return 0;
		}
	}
	formulas.push_back(tempFormula);

	for (i = 0; i < int(formulas.size()); i++)
	{
		if (!isCorrect0_1_2Formula(formulas[i]))
		{
			return 0;
		}
	}

	return 1;
}

int Parser::getFormulaIndex(QString& formula, int start)
{
	int result = 0;

	if (formula[start] == '{')
	{
		start++;
	}

	while (formula[start] != '}')
	{
		result *= 10;
		result += QString(formula[start]).toInt();
		start++;
	}

	return result;
}

QString	Parser::intToString(int number)
{
	if (number == 0)
	{
		return "0";
	}

	QString result;
	while (number > 0)
	{
		result += char(number % 10 + '0');
		number /= 10;
	}

	reverse(result.begin(), result.end());

	return result;
}

Formula* Parser::parseFormula(QString formula)
{
	std::vector<Formula*> formulas;

	// process parenthesis
	for (int i = 0; i < int(formula.size()); i++)
	{
		if (formula[i] == '(')
		{
			QString insideFormula;
			int start_ = i, end_ = i + 1, count = 1;
			for (end_ = i + 1;; end_++)
			{
				if (formula[end_] == '(')
				{
					count++;
				}
				if (formula[end_] == ')')
				{
					count--;
				}
				if (count == 0)
				{
					break;
				}
				insideFormula += formula[end_];
			}

			int formulaIndex = formulas.size();

			formulas.push_back(parseFormula(insideFormula));

			int quantifierI;
			for (quantifierI = i - 1; quantifierI >= 0; quantifierI--)
			{
				if (isVariableSymbol(formula[quantifierI]))
				{
					quantifierI--;

					if (formula[quantifierI] == QChar(existSymbol))
					{
						formulas.back()->quantifiers.insert(formulas.back()->quantifiers.begin(), 
							Quantifier(QUANTIFIER::exist, QString(formula[quantifierI + 1])));
					}
					else
					{
						formulas.back()->quantifiers.insert(formulas.back()->quantifiers.begin(),
							Quantifier(QUANTIFIER::forany, QString(formula[quantifierI + 1])));
					}
				}
				else
				{
					break;
				}
			}

			QString newFormula;
			for (int j = 0; j <= quantifierI; j++)
			{
				newFormula += formula[j];
			}
			newFormula += "{";
			newFormula += intToString(formulaIndex);
			newFormula += "}";
			i = newFormula.size();
			i--;
			for (int j = end_ + 1; j < int(formula.size()); j++)
			{
				newFormula += formula[j];
			}

			formula = newFormula;
		}
	}

	// process predicate
	for (int i = 0; i < int(formula.size()); i++)
	{
		if (isPredicateSymbol(formula[i]))
		{
			int formulaIndex = formulas.size(), pos = i;

			formulas.push_back(new Predicate());
			((Predicate*)formulas.back())->name = QString(formula[i]);

			int quantifierI;
			for (quantifierI = i - 1; quantifierI >= 0; quantifierI--)
			{
				if (isVariableSymbol(formula[quantifierI]))
				{
					quantifierI--;

					if (formula[quantifierI] == QChar(existSymbol))
					{
						formulas.back()->quantifiers.insert(formulas.back()->quantifiers.begin(), 
							Quantifier(QUANTIFIER::exist, QString(formula[quantifierI + 1])));
					}
					else
					{
						formulas.back()->quantifiers.insert(formulas.back()->quantifiers.begin(), 
							Quantifier(QUANTIFIER::forany, QString(formula[quantifierI + 1])));
					}
				}
				else
				{
					break;
				}
			}

			int parametersI;
			for (parametersI = i + 1; formula[parametersI] != ']'; parametersI++)
			{
				if (isVariableSymbol(formula[parametersI]))
				{
					((Predicate*)formulas.back())->parameters.push_back(new Variable);
					((Variable*)(((Predicate*)formulas.back())->parameters.back()))->name = QString(formula[parametersI]);
				}
			}

			QString newFormula;
			for (int j = 0; j <= quantifierI; j++)
			{
				newFormula += formula[j];
			}
			newFormula += "{";
			newFormula += intToString(formulaIndex);
			newFormula += "}";
			i = newFormula.size();
			i--;
			for (int j = parametersI + 1; j < int(formula.size()); j++)
			{
				newFormula += formula[j];
			}

			formula = newFormula;
		}
	}

	// process negation
	for (int i = int(formula.size() - 1); i >= 0; i--)
	{
		if (formula[i] == QChar(negationSymbol))
		{
			int start_ = i + 1, end_ = i + 2;
			while (formula[end_] != '}')
			{
				end_++;
			}

			int childFormulaIndex = getFormulaIndex(formula, start_);
			int formulaIndex = formulas.size();

			formulas.push_back(new Negation);
			((Negation*)formulas.back())->child = formulas[childFormulaIndex];

			int quantifierI;
			for (quantifierI = i - 1; quantifierI >= 0; quantifierI--)
			{
				if (isVariableSymbol(formula[quantifierI]))
				{
					quantifierI--;

					if (formula[quantifierI] == QChar(existSymbol))
					{
						formulas.back()->quantifiers.insert(formulas.back()->quantifiers.begin(),
							Quantifier(QUANTIFIER::exist, QString(formula[quantifierI + 1])));
					}
					else
					{
						formulas.back()->quantifiers.insert(formulas.back()->quantifiers.begin(),
							Quantifier(QUANTIFIER::forany, QString(formula[quantifierI + 1])));
					}
				}
				else
				{
					break;
				}
			}

			QString newFormula;
			for (int j = 0; j <= quantifierI; j++)
			{
				newFormula += formula[j];
			}
			newFormula += "{";
			newFormula += intToString(formulaIndex);
			newFormula += "}";
			for (int j = end_ + 1; j < int(formula.size()); j++)
			{
				newFormula += formula[j];
			}

			formula = newFormula;
		}
	}

	//process conjunction
	for (int i = 0; i < int(formula.size()); i++)
	{
		if (formula[i] == QChar(conjunctionSymbol))
		{
			int start1 = i - 2, end1 = i - 1;
			while (formula[start1] != '{')
			{
				start1--;
			}

			int start2 = i + 1, end2 = i + 2;
			while (formula[end2] != '}')
			{
				end2++;
			}

			int child1FormulaIndex = getFormulaIndex(formula, start1);
			int child2FormulaIndex = getFormulaIndex(formula, start2);
			int formulaIndex = formulas.size();

			formulas.push_back(new Conjunction);
			((Conjunction*)formulas.back())->children.push_back(formulas[child1FormulaIndex]);
			((Conjunction*)formulas.back())->children.push_back(formulas[child2FormulaIndex]);

			QString newFormula;
			for (int j = 0; j < start1; j++)
			{
				newFormula += formula[j];
			}
			newFormula += "{";
			newFormula += intToString(formulaIndex);
			newFormula += "}";
			i = newFormula.size();
			i--;
			for (int j = end2 + 1; j < int(formula.size()); j++)
			{
				newFormula += formula[j];
			}

			formula = newFormula;
		}
	}

	//process disjunction
	for (int i = 0; i < int(formula.size()); i++)
	{
		if (formula[i] == QChar(disjunctionSymbol))
		{
			int start1 = i - 2, end1 = i - 1;
			while (formula[start1] != '{')
			{
				start1--;
			}

			int start2 = i + 1, end2 = i + 2;
			while (formula[end2] != '}')
			{
				end2++;
			}

			int child1FormulaIndex = getFormulaIndex(formula, start1);
			int child2FormulaIndex = getFormulaIndex(formula, start2);
			int formulaIndex = formulas.size();

			formulas.push_back(new Disjunction);
			((Disjunction*)formulas.back())->children.push_back(formulas[child1FormulaIndex]);
			((Disjunction*)formulas.back())->children.push_back(formulas[child2FormulaIndex]);

			QString newFormula;
			for (int j = 0; j < start1; j++)
			{
				newFormula += formula[j];
			}
			newFormula += "{";
			newFormula += intToString(formulaIndex);
			newFormula += "}";
			i = newFormula.size();
			i--;
			for (int j = end2 + 1; j < int(formula.size()); j++)
			{
				newFormula += formula[j];
			}

			formula = newFormula;
		}
	}

	//process implication
	for (int i = 0; i < int(formula.size()); i++)
	{
		if (formula[i] == QChar(implicationSymbol))
		{
			int start1 = i - 2, end1 = i - 1;
			while (formula[start1] != '{')
			{
				start1--;
			}

			int start2 = i + 2, end2 = i + 3;
			while (formula[end2] != '}')
			{
				end2++;
			}

			int child1FormulaIndex = getFormulaIndex(formula, start1);
			int child2FormulaIndex = getFormulaIndex(formula, start2);

			formulas.push_back(new Implication);
			((Implication*)formulas.back())->child1 = formulas[child1FormulaIndex];
			((Implication*)formulas.back())->child2 = formulas[child2FormulaIndex];

			QString newFormula;
			for (int j = 0; j < start1; j++)
			{
				newFormula += formula[j];
			}
			newFormula += "{";
			newFormula += intToString(int(formulas.size()) - 1);
			newFormula += "}";
			i = newFormula.size();
			i--;
			for (int j = end2 + 1; j < int(formula.size()); j++)
			{
				newFormula += formula[j];
			}

			formula = newFormula;
		}
	}

	return formulas[getFormulaIndex(formula, 0)];
}

vector<Formula*> Parser::parseInput(QString input)
{
	{
		int i;
		QString tempFormula;

		for (i = 0; i < int(input.size()); i++)
		{
			if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
			{
				tempFormula += input[i];
			}
		}

		input = tempFormula;
	}

	for (int i = 0; i+1 < int(input.size()); i++)
	{
		if (input[i] == QChar(negationSymbol))
		{
			if (input[i + 1] == QChar(negationSymbol))
			{
				QString tempString;

				for (int j = 0; j< int(input.size()); j++)
				{
					if (j != i && j != (i+1))
					{
						tempString += input[j];
					}
				}
				
				input = tempString;

				i--;
			}
			else if (isQuantifierSymbol(input[i + 1]))
			{
				swap(input[i], input[i + 1]);
				swap(input[i + 1], input[i + 2]);

				if (input[i] == QChar(existSymbol))
				{
					input[i] = QChar(foranySymbol);
				}
				else
				{
					input[i] = QChar(existSymbol);
				}
			}
		}
	}

	int parameterBrackets = 0;
	for (int i = 0; i < int(input.size()); i++)
	{
		if (parameterBrackets == 1 && input[i] == ')')
		{
			parameterBrackets = 0;
			input[i] = ']';

			continue;
		}

		if (i + 1 == int(input.size()))
		{
			if (isPredicateSymbol(input[i]))
			{
				input += "[]";
			}

			continue;
		}

		if (isPredicateSymbol(input[i]))
		{
			if (input[i + 1] != '(')
			{
				int j;
				QString tempFormula;
				for (j = 0; j <= i; j++)
				{
					tempFormula += input[i];
				}
				tempFormula += "[]";
				for (; j < input.size(); j++)
				{
					tempFormula += input[j];
				}

				input = tempFormula;
			}
			else
			{
				input[i + 1] = '[';
				parameterBrackets = 1;
			}
		}
	}

	int isRightSide = 0;

	std::vector<Formula*> result;

	for (int i = 0; i < int(input.size()); i++)
	{
		if (input[i] == '=')
		{
			isRightSide = 1;

			continue;
		}

		if (input[i] == '|')
		{
			isRightSide = 1;
			i++;

			continue;
		}

		if (input[i] == ',')
		{
			continue;
		}

		QString formula;
		int statr_ = i, end_ = i, isParam = 0;

		while (end_ < int(input.size()) && (input[end_] != ',' || isParam == 1) && input[end_] != '|')
		{
			if (input[end_] == '[')
			{
				isParam = 1;
			}
			if (input[end_] == ']')
			{
				isParam = 0;
			}

			formula += input[end_];
			end_++;
		}
		i = end_ - 1;

		if (isRightSide == 1)
		{
			formula = QString(QChar(negationSymbol)) + "(" + formula + ")";
		}

		Formula* root = parseFormula(formula);

		result.push_back(root);
	}

	return result;
}
