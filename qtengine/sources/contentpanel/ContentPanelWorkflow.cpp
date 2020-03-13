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
#include "BuiltIn.hpp"

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

	_registryBuiltIn = generateRegistryBuiltIn();

	splitter->addWidget(_tree);
	splitter->addWidget(_view);
	splitter->setSizes({_tree->sizeHint().width(), _tree->sizeHint().width() * 4});

	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelWorkflow::onViewObjectChanged);

	_tree->onViewObjectClassChanged(Manager::instance()->viewManager()->viewObjectClass());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectClassChanged, _tree, &TreeWidgetWorkflow::onViewObjectClassChanged);
}

std::shared_ptr<QtNodes::DataModelRegistry> qtengine::ContentPanelWorkflow::generateRegistryBuiltIn() const
{
	auto registry = std::make_shared<QtNodes::DataModelRegistry>();

	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Int)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Double)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Bool)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::String)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Date)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Time)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::DateTime)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::KeySequence)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Char)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Locale)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Point)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::PointF)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Size)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::SizeF)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Rect)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::RectF)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Color)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::SizePolicy)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Font)); });
	registry->registerModel<BuiltIn>("Built-in", []() { return std::unique_ptr<BuiltIn>(new BuiltIn(QVariant::Cursor)); });
	return registry;
}

std::shared_ptr<QtNodes::DataModelRegistry> qtengine::ContentPanelWorkflow::generateRegistryView(const QMetaObject *metaObject, QMetaMethod::Access minimumAccess) const
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
	auto registryView = generateRegistryView(viewObject->object()->metaObject(), QMetaMethod::Protected);
	auto registry = std::make_shared<QtNodes::DataModelRegistry>();

	registry->concatenate(registryView.get());
	registry->concatenate(_registryBuiltIn.get());
	_scene->setRegistry(registry);
}
