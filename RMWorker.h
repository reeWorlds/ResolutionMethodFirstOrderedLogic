#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RMWorker.h"

#include "Formula.h"
#include "Disjunction.h"
#include "Conjunction.h"
#include "Negation.h"
#include "Implacation.h"
#include "Predicate.h"
#include "Parser.h"

#include "FormulaTransformator.h"
#include "FormulaConverter.h"
#include "RMImplementer.h"

#include <vector>
#include <map>
#include <QString>
#include <string>

class RMWorker : public QMainWindow
{
	Q_OBJECT

public:
	RMWorker(QWidget *parent = Q_NULLPTR);

private:
	Ui::RMWorkerClass ui;

public slots:

	void addExist();
	void addForany();
	void addDisjunction();
	void addConjunction();
	void addImplication();
	void addNegation();

	void apply();
};
