#include "RMWorker.h"
#include <QtWidgets/QApplication>

#include <clocale>

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE, "ukr");

	QApplication a(argc, argv);
	RMWorker w;
	w.show();
	return a.exec();
}
