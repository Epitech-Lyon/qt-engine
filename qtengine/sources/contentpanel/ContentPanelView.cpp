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

qtengine::ContentPanelView::~ContentPanelView()
{
	clear();
}

void qtengine::ContentPanelView::init()
{
	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelView::onViewObjectChanged);
}

void qtengine::ContentPanelView::onViewObjectChanged(libraryObjects::AObject *object)
{
	clear();
	if (object) {
		if (object->object()->inherits("QWidget"))
			_mainLayout->addWidget(dynamic_cast<QWidget*>(object->object()));
		else if (object->object()->inherits("QLayout"))
			_mainLayout->addLayout(dynamic_cast<QLayout*>(object->object()));
	}
}

void qtengine::ContentPanelView::clear()
{
	while (!_mainLayout->isEmpty()) {
		auto item = _mainLayout->itemAt(0);

		_mainLayout->removeItem(item);
		if (item->widget())
			item->widget()->setParent(nullptr);
	}
}
