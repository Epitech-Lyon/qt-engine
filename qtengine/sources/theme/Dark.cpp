/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Dark
*/

#include "Dark.hpp"

#include <QFile>
#include <QStyleFactory>

qtengine::Dark::Dark()
	: Dark(styleBase())
{
}

qtengine::Dark::Dark(QStyle *style)
	: QProxyStyle(style)
{
}

QStyle *qtengine::Dark::styleBase(QStyle *style) const
{
	return !style ? QStyleFactory::create("Fusion") : style;
}

QStyle *qtengine::Dark::baseStyle() const
{
	return styleBase();
}

void qtengine::Dark::polish(QApplication *app)
{
	if (!app) { return; }
	QFile file(":theme/dark/style.qss");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		app->setStyleSheet(QString::fromLatin1(file.readAll()));
		file.close();
	}
}
