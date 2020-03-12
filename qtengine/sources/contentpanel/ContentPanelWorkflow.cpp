/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#include "moc_ContentPanelWorkflow.cpp"
#include "ContentPanelWorkflow.hpp"

#include "TreeWidgetWorkflow.hpp"
#include "FlowView.hpp"
#include "FlowScene.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "AObject.hpp"
#include "ObjectClass.hpp"

#include "Utils.hpp"
#include "Constructor.hpp"
#include "Method.hpp"
#include "Property.hpp"

#include <QtWidgets/QSplitter>
#include <QtWidgets/QGraphicsSceneDragDropEvent>
#include <QtCore/QDebug>

qtengine::FlowScene::FlowScene(QObject *parent)
	: QtNodes::FlowScene(parent)
{
}

void qtengine::FlowScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	QtNodes::FlowScene::dragEnterEvent(event);
}

#include "LibraryObjectMimeData.hpp"
void qtengine::FlowScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
//	QtNodes::FlowScene::dragMoveEvent(event);
	qDebug() << event->mimeData() << dynamic_cast<const libraryObjects::LibraryObjectMimeData*>(event->mimeData());
	event->accept();
}

void qtengine::FlowScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	QtNodes::FlowScene::dropEvent(event);
}

qtengine::ContentPanelWorkflow::ContentPanelWorkflow(QWidget *parent)
	: ContentPanelBase("Workflow", parent)
{
}

void qtengine::ContentPanelWorkflow::init()
{
	auto splitter = new QSplitter(this);
	_mainLayout->addWidget(splitter);

	_tree = new TreeWidgetWorkflow(splitter);
	_scene = new FlowScene(splitter);
	_view = new QtNodes::FlowView(_scene, splitter);

	splitter->addWidget(_tree);
	splitter->addWidget(_view);
	splitter->setSizes({_tree->sizeHint().width(), _tree->sizeHint().width() * 4});

	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelWorkflow::onViewObjectChanged);

	_tree->onViewObjectClassChanged(Manager::instance()->viewManager()->viewObjectClass());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectClassChanged, _tree, &TreeWidgetWorkflow::onViewObjectClassChanged);
}

std::shared_ptr<QtNodes::DataModelRegistry> qtengine::ContentPanelWorkflow::generateRegistry(const QMetaObject *metaObject, QMetaMethod::Access minimumAccess)
{
	auto registry = std::make_shared<QtNodes::DataModelRegistry>();
	QString prefix("base class/");

	for (int idx = 0; idx < metaObject->constructorCount(); idx += 1) {
		auto metaMethod = metaObject->constructor(idx);

		if (metaMethod.access() >= minimumAccess)
			registry->registerModel<Method>(prefix + types::typeToString(metaMethod.methodType()), [=]() { return std::unique_ptr<Method>(new Method(metaMethod)); });
	}
	for (int idx = 0; idx < metaObject->methodCount(); idx += 1) {
		auto metaMethod = metaObject->method(idx);

		if (metaMethod.access() >= minimumAccess)
			registry->registerModel<Method>(prefix + types::typeToString(metaMethod.methodType()), [=]() { return std::unique_ptr<Method>(new Method(metaMethod)); });
	}
	for (int idx = 0; idx < metaObject->propertyCount(); idx += 1) {
		auto metaProperty = metaObject->property(idx);

		registry->registerModel<Property>(prefix + "Property", [=]() { return std::unique_ptr<Property>(new Property(metaProperty)); });
	}
	return registry;
}

void qtengine::ContentPanelWorkflow::onViewObjectChanged(libraryObjects::AObject *viewObject)
{
	_tree->clear();
	_scene->clearScene();
	if (!viewObject) { return; }
	_viewRegistry = generateRegistry(viewObject->object()->metaObject(), QMetaMethod::Protected);

	_scene->setRegistry(_viewRegistry);
}
