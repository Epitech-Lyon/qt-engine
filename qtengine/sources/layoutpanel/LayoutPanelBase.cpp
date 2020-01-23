/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelBase
*/

#include "LayoutPanelBase.hpp"

#include "MainWindow.hpp"

qtengine::LayoutPanelBase::LayoutPanelBase(QWidget *parent)
    : QWidget(parent)
	, _mainLayout(new QVBoxLayout(this))
{
	_mainLayout->setMargin(2);
	setLayout(_mainLayout);
}

qtengine::LayoutPanelBase *qtengine::LayoutPanelBase::parentLayoutPanel()
{
	QWidget *parent = parentWidget();

	while (!dynamic_cast<LayoutPanelBase*>(parent) && !dynamic_cast<MainWindow*>(parent))
		parent = parent->parentWidget();
	return dynamic_cast<LayoutPanelBase*>(parent);
}

void qtengine::LayoutPanelBase::setChildPanel(LayoutPanelBase *layoutPanel)
{
	while (_mainLayout->count() > 0)
		_mainLayout->removeItem(_mainLayout->itemAt(0));
	_mainLayout->addWidget(layoutPanel);
}
