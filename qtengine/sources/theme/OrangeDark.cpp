/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** OrangeDark
*/

#include "OrangeDark.hpp"

#include <QFile>
#include <QStyleFactory>

qtengine::OrangeDark::OrangeDark()
	: OrangeDark(styleBase())
{
}

qtengine::OrangeDark::OrangeDark(QStyle *style)
	: QProxyStyle(style)
{
}

QStyle *qtengine::OrangeDark::styleBase(QStyle *style) const
{
	return !style ? QStyleFactory::create("Fusion") : style;
}

QStyle *qtengine::OrangeDark::baseStyle() const
{
	return styleBase();
}

void qtengine::OrangeDark::polish(QApplication *app)
{
	if (!app) { return; }
	QFile file(":theme/orangedark/style.qss");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		app->setStyleSheet(QString::fromLatin1(file.readAll()));
		file.close();
	}
}
