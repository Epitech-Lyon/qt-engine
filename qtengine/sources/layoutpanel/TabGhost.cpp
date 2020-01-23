/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TabGhost
*/

#include "TabGhost.hpp"

#include "TabWidget.hpp"
#include "TabBar.hpp"
#include <QtGui/QPalette>
#include <QtWidgets/QApplication>

qtengine::TabGhost::TabGhost(const QPoint &pos, TabBar *tabBar)
	: QWidget(nullptr, Qt::FramelessWindowHint)
	, _index(tabBar->currentIndex())
	, _offset(tabBar->mapToGlobal(pos) - tabBar->mapToGlobal(QPoint(0, 0)) - QPoint(getWidth(tabBar), 0))
	, _originPos(tabBar->mapToGlobal(pos))
	, _geometry(dynamic_cast<QWidget *>(tabBar->parent())->geometry())
{
	QPalette palette;
	auto tabWidgetGhost = createTabGhost(tabBar);

	palette.setBrush(this->backgroundRole(), QBrush(tabWidgetGhost->tabBar()->grab()));
	setPalette(palette);
	setGeometry(tabWidgetGhost->tabBar()->geometry());
	setWindowOpacity(0.5);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	moveWithOffset(tabBar->mapToGlobal(pos));
	delete tabWidgetGhost;
}

qtengine::TabWidget *qtengine::TabGhost::createTabGhost(TabBar *tabBar)
{
	auto parent = dynamic_cast<TabWidget *>(tabBar->parent());
	auto tabWidgetGhost = new TabWidget;

	tabWidgetGhost->QTabWidget::addTab(new QWidget(tabWidgetGhost), parent->tabText(_index));
	tabWidgetGhost->setGeometry(parent->geometry());
	tabWidgetGhost->setWindowOpacity(0);
	tabWidgetGhost->show();
	return tabWidgetGhost;
}

int qtengine::TabGhost::getWidth(TabBar *tabBar)
{
	auto parent = dynamic_cast<TabWidget *>(tabBar->parent());
	TabWidget tabWidgetGhost;

	for (int i = 0; i < _index; i += 1)
		tabWidgetGhost.QTabWidget::addTab(new QWidget(&tabWidgetGhost), parent->tabText(i));
	tabWidgetGhost.setGeometry(parent->geometry());
	tabWidgetGhost.setWindowOpacity(0);
	tabWidgetGhost.show();
	return tabWidgetGhost.tabBar()->width();
}

void qtengine::TabGhost::moveWithOffset(const QPoint &pos)
{
	move(pos - _offset);
}

bool qtengine::TabGhost::dragStarted(const QPoint &pos)
{
	return (pos - _originPos).manhattanLength() >= QApplication::startDragDistance();
}
