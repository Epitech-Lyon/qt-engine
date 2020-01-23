/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** BlueDark
*/

#include "BlueDark.hpp"

#include <QFile>
#include <QStyleFactory>

qtengine::BlueDark::BlueDark()
	: BlueDark(styleBase())
{
}

qtengine::BlueDark::BlueDark(QStyle *style)
	: QProxyStyle(style)
{
}

QStyle *qtengine::BlueDark::styleBase(QStyle *style) const
{
	return !style ? QStyleFactory::create("Fusion") : style;
}

QStyle *qtengine::BlueDark::baseStyle() const
{
	return styleBase();
}

void qtengine::BlueDark::polish(QPalette &palette)
{
	palette.setColor(QPalette::Window, QColor(53, 53, 53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
	palette.setColor(QPalette::Base, QColor(42, 42, 42));
	palette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
	palette.setColor(QPalette::ToolTipBase, Qt::white);
	palette.setColor(QPalette::ToolTipText, QColor(53, 53, 53));
	palette.setColor(QPalette::Text, Qt::white);
	palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
	palette.setColor(QPalette::Dark, QColor(35, 35, 35));
	palette.setColor(QPalette::Shadow, QColor(20, 20, 20));
	palette.setColor(QPalette::Button, QColor(53, 53, 53));
	palette.setColor(QPalette::ButtonText, Qt::white);
	palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
	palette.setColor(QPalette::BrightText, Qt::red);
	palette.setColor(QPalette::Link, QColor(42, 130, 218));
	palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
	palette.setColor(QPalette::HighlightedText, Qt::white);
	palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
}

void qtengine::BlueDark::polish(QApplication *app)
{
	if (!app) { return; }
	QFile file(":theme/bluedark/style.qss");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		app->setStyleSheet(QString::fromLatin1(file.readAll()));
		file.close();
	}
}
