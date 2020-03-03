/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TabBar
*/

#include "TabBar.hpp"

#include "TabGhost.hpp"
#include "TabWidget.hpp"
#include "LayoutPanelTabber.hpp"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QApplication>

qtengine::TabBar::TabBar(QWidget *parent)
	: QTabBar(parent)
	, _tabGhost(nullptr)
	, _isDragging(false)
{
}

void qtengine::TabBar::mouseMoveEvent(QMouseEvent *event)
{
	QPoint point = event->pos() - _mousePressedPos;

	if (!_isDragging && point.manhattanLength() > 5 && tabAt(event->pos()) != -1) {
		_isDragging = true;
		_tabGhost = new TabGhost(event->pos(), this);
		_tabGhost->show();
	}
	if (_tabGhost)
		_tabGhost->moveWithOffset(event->globalPos());
}

void qtengine::TabBar::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && tabAt(event->pos()) > -1)
		_mousePressedPos = event->pos();
	QTabBar::mousePressEvent(event);
}

void qtengine::TabBar::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton) {
		QTabBar::mouseReleaseEvent(event);
		return;
	}
	if (_isDragging) {
		_isDragging = false;
		auto tabBar = dynamic_cast<TabBar*>(QApplication::widgetAt(event->globalPos()));

		if (tabBar) {
			auto tabWidget = dynamic_cast<TabWidget*>(tabBar->parent());
			auto layoutPanelTabber = dynamic_cast<LayoutPanelTabber*>(tabWidget->parent());

			if (layoutPanelTabber)
				moveToWindow(layoutPanelTabber, event->globalPos());
		}
		delete _tabGhost;
		_tabGhost = nullptr;
	}
}

void qtengine::TabBar::moveToWindow(LayoutPanelTabber *layoutPanelTabber, const QPoint &pos)
{
	auto oldTabWidget = dynamic_cast<TabWidget*>(parent());
	auto oldPanelTabber = dynamic_cast<LayoutPanelTabber*>(oldTabWidget->parent());
	int index = _tabGhost->index();

	if (oldPanelTabber == layoutPanelTabber && index == oldTabWidget->tabBar()->tabAt(oldTabWidget->mapFromGlobal(pos)))
		return;
	layoutPanelTabber->setCurrentTab(layoutPanelTabber->insertTab(pos, oldPanelTabber->closeTab(index)));
}
