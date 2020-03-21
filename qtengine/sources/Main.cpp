/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** main
*/

#include <QtWidgets/QApplication>
#include "Manager.hpp"
#include "MainWindow.hpp"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QRegExp>

void rxObjClassName()
{
	QString code("E_OBJCLASSNAME(E0)_E::function()");

	QRegExp regex("E_OBJCLASSNAME\\((I|O|M)([0-9])\\)_E");
	int pos = regex.indexIn(code);
	if (pos != -1) {
		qDebug() << regex.cap(1) << regex.cap(2);
		qDebug() << code.replace(pos, regex.matchedLength(), "ObjectClassName");
	}
}

void rxObjName()
{
	QString code("E_OBJNAME(E0)_E->function()");

	QRegExp regex("E_OBJNAME\\((I|O|M)([0-9])\\)_E");
	int pos = regex.indexIn(code);
	if (pos != -1) {
		qDebug() << regex.cap(1) << regex.cap(2);
		qDebug() << code.replace(pos, regex.matchedLength(), "ObjectName");
	}
}

void rxVar()
{
	QString code("int E_VAR(coucou)_E = function()");

	QRegExp regex("E_VAR\\((\\w*)\\)_E");
	int pos = regex.indexIn(code);
	if (pos != -1) {
		qDebug() << regex.cap(1);
		qDebug() << code.replace(pos, regex.matchedLength(), "val_1");
	}
}

void rxUseVar()
{
	QString code("int test = E_USEVAR(1)_E");

	QRegExp regex("E_USEVAR\\(([0-9])\\)_E");
	int pos = regex.indexIn(code);
	if (pos != -1) {
		qDebug() << regex.cap(1);
		qDebug() << code.replace(pos, regex.matchedLength(), "val_1");
	}
}

void rxCode()
{
	QString code("if (E_CODE(I1)_E) {\n"
	"E_CODE(O0)_E"
	"} else {\n"
	"E_CODE(O1)_E"
	"}\n");

	QRegExp regex("E_CODE\\((I|O)([0-9])\\)_E");
	int pos = 0;
	while ((pos = regex.indexIn(code, pos)) != -1) {
		qDebug() << regex.cap(1) << regex.cap(2);
		pos += regex.matchedLength();
	}
}

int main(int ac, char **av)
{
//	rxObjClassName();
//	rxObjName();
//	rxVar();
//	rxUseVar();
//	rxCode();
//	return 0;
	QApplication app(ac, av);
	auto manager = qtengine::Manager::instance();

	manager->init();
	manager->mainWindow()->showMaximized();
	return app.exec();
}
