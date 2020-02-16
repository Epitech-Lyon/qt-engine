/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelView
*/

#include "ContentPanelView.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "AObject.hpp"

qtengine::ContentPanelView::ContentPanelView(QWidget *parent)
	: ContentPanelBase("View", parent)
{
}

void qtengine::ContentPanelView::init()
{
	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelView::onViewObjectChanged);
}

void qtengine::ContentPanelView::onViewObjectChanged(libraryObjects::AObject *object)
{
	while (!_mainLayout->isEmpty())
		_mainLayout->removeItem(_mainLayout->itemAt(0));
	if (object)
		_mainLayout->addWidget(dynamic_cast<QWidget*>(object->object()));
}
