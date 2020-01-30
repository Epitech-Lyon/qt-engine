/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewExplorer
*/

#include "ContentPanelViewExplorer.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"

qtengine::ContentPanelViewExplorer::ContentPanelViewExplorer(QWidget *parent)
	: ContentPanelBase("View Explorer", parent)
{
}

void qtengine::ContentPanelViewExplorer::init()
{
	_treeWidget = new QTreeWidget(this);
	_treeWidget->setHeaderLabels({"Object name", "Class"});
	_mainLayout->addWidget(_treeWidget);

	auto onViewObjectChanged = [this](const QJsonObject &json) {
		_treeWidget->clear();
		buildTree(_treeWidget->invisibleRootItem(), json);
		_treeWidget->expandAll();
	};

	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, onViewObjectChanged);
}

QToolBar *qtengine::ContentPanelViewExplorer::initToolBar()
{
	return nullptr;
}

void qtengine::ContentPanelViewExplorer::buildTree(QTreeWidgetItem *parent, const QJsonObject &json)
{
	auto className = json.keys().front();
	auto jsonObject = json[className].toObject();
	auto objectName = jsonObject["Properties"].toObject()["objectName"].toString();
	auto widgetItem = new QTreeWidgetItem(parent, {objectName, className});

	for (auto jsonChild : jsonObject["Children"].toArray())
		buildTree(widgetItem, jsonChild.toObject());
}
