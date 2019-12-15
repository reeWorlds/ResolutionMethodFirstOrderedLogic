#include "RMWorker.h"

RMWorker::RMWorker(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.existButton, SIGNAL(clicked()), this, SLOT(addExist()));
	connect(ui.foranyButton, SIGNAL(clicked()), this, SLOT(addForany()));
	connect(ui.disjunctionButton, SIGNAL(clicked()), this, SLOT(addDisjunction()));
	connect(ui.conjunctionButton, SIGNAL(clicked()), this, SLOT(addConjunction()));
	connect(ui.implicationButton, SIGNAL(clicked()), this, SLOT(addImplication()));
	connect(ui.negationButton, SIGNAL(clicked()), this, SLOT(addNegation()));

	connect(ui.todoPushButton, SIGNAL(clicked()), this, SLOT(apply()));
}

void RMWorker::addExist()
{
	int cursorPosition = ui.inputLine->cursorPosition();
	QString tempString = ui.inputLine->text();
	tempString.insert(cursorPosition, QChar(0x2203));

	ui.inputLine->setText(tempString);
	ui.inputLine->setCursorPosition(cursorPosition + 1);

	ui.inputLine->setFocus();
}

void RMWorker::addForany()
{
	int cursorPosition = ui.inputLine->cursorPosition();
	QString tempString = ui.inputLine->text();
	tempString.insert(cursorPosition, QChar(0x2200));

	ui.inputLine->setText(tempString);
	ui.inputLine->setCursorPosition(cursorPosition + 1);

	ui.inputLine->setFocus();
}

void RMWorker::addDisjunction()
{
	int cursorPosition = ui.inputLine->cursorPosition();
	QString tempString = ui.inputLine->text();
	tempString.insert(cursorPosition, QChar(0x2228));

	ui.inputLine->setText(tempString);
	ui.inputLine->setCursorPosition(cursorPosition + 1);

	ui.inputLine->setFocus();
}

void RMWorker::addConjunction()
{
	int cursorPosition = ui.inputLine->cursorPosition();
	QString tempString = ui.inputLine->text();
	tempString.insert(cursorPosition, QChar(0x2227));

	ui.inputLine->setText(tempString);
	ui.inputLine->setCursorPosition(cursorPosition + 1);

	ui.inputLine->setFocus();
}

void RMWorker::addImplication()
{
	int cursorPosition = ui.inputLine->cursorPosition();
	QString tempString = ui.inputLine->text();
	tempString.insert(cursorPosition, QChar(0x2192));

	ui.inputLine->setText(tempString);
	ui.inputLine->setCursorPosition(cursorPosition + 1);

	ui.inputLine->setFocus();
}

void RMWorker::addNegation()
{
	int cursorPosition = ui.inputLine->cursorPosition();
	QString tempString = ui.inputLine->text();
	tempString.insert(cursorPosition, QChar(0x00AC));

	ui.inputLine->setText(tempString);
	ui.inputLine->setCursorPosition(cursorPosition + 1);

	ui.inputLine->setFocus();
}

void RMWorker::apply()
{
	QString inputString = ui.inputLine->text();

	QString output;

	if (!Parser::isCorrectFormula(inputString))
	{
		QString tempString = QString::fromStdWString(L"Вхідна формула не є коректною!");
		ui.textInfo->setText(tempString);

		return;
	}

	// parse
	vector<Formula*> formulas = Parser::parseInput(inputString);

	output += QString::fromStdWString(L"Необхідно довести суперечність системи наступних формул:\n");
	for (auto& it : formulas)
	{
		output += FormulaConverter::formulaToString(it) + ", ";
	}
	output += "\n\n";

	// replace implication
	{
		vector<Formula*> tempFormulas;

		for (auto& it : formulas)
		{
			tempFormulas.push_back(FormulaTransformator::replaceImplication(it));
		}

		formulas = tempFormulas;
	}

	// make variables unigue
	map<QChar, int> variablesData;
	for (auto& it : formulas)
	{
		FormulaTransformator::unifyVariables(variablesData, it);
	}

	output += QString::fromStdWString(L"Деуніфікуємо змінні:\n");
	for (auto& it : formulas)
	{
		output += FormulaConverter::formulaToString(it) + ", ";
	}
	output += "\n\n";

	// make preneks form
	for (auto& it : formulas)
	{
		FormulaTransformator::pushQuantifiers(it);
	}

	output += QString::fromStdWString(L"Зводимо до пренексної форми:\n");
	for (auto& it : formulas)
	{
		output += FormulaConverter::formulaToString(it) + ", ";
	}
	output += "\n\n";

	// delete all quanfiers
	int functionCount = 0, variableCount = 0;
	
	for (auto& it : formulas)
	{
		FormulaTransformator::removeQuantifiers(it, functionCount, variableCount);
	}
	output += QString::fromStdWString(L"Сколемізуємо:\n");
	for (auto& it : formulas)
	{
		output += FormulaConverter::formulaToString(it) + ", ";
	}
	output += "\n\n";

	// make disjuncts
	vector<Disjunct> disjuncts;
	for (auto& it : formulas)
	{
		vector<Disjunct> tempDisjuncts = FormulaTransformator::simplifyFormula(it);

		for (auto& itt : tempDisjuncts)
		{
			disjuncts.push_back(itt);
		}
	}

	output += QString::fromStdWString(L"Розкладаємо на диз'юнкти:\n");
	for (auto& it : disjuncts)
	{
		output += it.toString();
		output += ", ";
	}
	output += "\n\n";

	// make all possible dijuncts of constants
	vector<Disjunct> constantDisjuncts = RMImplementer::makeConstantDisjuncts(disjuncts);

	// to prove or not to prove
	QString proofString = RMImplementer::tryToProve(constantDisjuncts);
	output += proofString;

	// and finnaly output
	ui.textInfo->setText(output);
}