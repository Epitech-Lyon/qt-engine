/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewExplorer
*/

#include "ContentPanelViewExplorer.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"

#include "AObject.hpp"

qtengine::ContentPanelViewExplorer::ContentPanelViewExplorer(QWidget *parent)
	: ContentPanelBase("View Explorer", parent)
{
}

void qtengine::ContentPanelViewExplorer::init()
{
	_treeWidget = new QTreeWidget(this);
	_treeWidget->setHeaderLabels({"Object name", "Class"});
	_mainLayout->addWidget(_treeWidget);

	auto onViewObjectChanged = [this](libraryObjects::AObject *viewObject) {
		_treeWidget->clear();
		buildTree(_treeWidget->invisibleRootItem(), viewObject);
		_treeWidget->expandAll();
	};

	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, onViewObjectChanged);
}

QToolBar *qtengine::ContentPanelViewExplorer::initToolBar()
{
	return nullptr;
}

void qtengine::ContentPanelViewExplorer::buildTree(QTreeWidgetItem *parent, libraryObjects::AObject *object)
{
	if (!object) { return; }
	auto widgetItem = new QTreeWidgetItem(parent, {object->objectName(), object->className()});

	for (auto objectChild : object->children())
		buildTree(widgetItem, objectChild);
}
