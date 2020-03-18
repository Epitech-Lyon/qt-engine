/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#include "moc_ContentPanelWorkflow.cpp"
#include "ContentPanelWorkflow.hpp"

#include "TreeWidgetWorkflow.hpp"
#include "FlowSceneWorkflow.hpp"
#include "FlowView.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "AObject.hpp"
#include "ObjectClass.hpp"

#include "Utils.hpp"

#include "Method.hpp"
#include "Property.hpp"
#include "BuiltIn.hpp"

#include <QtWidgets/QSplitter>
#include <QtCore/QMetaEnum>

qtengine::ContentPanelWorkflow::ContentPanelWorkflow(QWidget *parent)
	: ContentPanelBase("Workflow", parent)
{
}

void qtengine::ContentPanelWorkflow::init()
{
	auto splitter = new QSplitter(this);
	_mainLayout->addWidget(splitter);

	_tree = new TreeWidgetWorkflow(splitter);
	_scene = new FlowSceneWorkflow(splitter);
	_view = new QtNodes::FlowView(_scene, splitter);

	_registryBuiltIn = generateRegistryBuiltIn();

	splitter->addWidget(_tree);
	splitter->addWidget(_view);
	splitter->setSizes({_tree->sizeHint().width(), _tree->sizeHint().width() * 4});

	onObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelWorkflow::onObjectChanged);

	onObjectClassChanged(Manager::instance()->viewManager()->viewObjectClass());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectClassChanged, this, &ContentPanelWorkflow::onObjectClassChanged);

	connect(_scene, &FlowSceneWorkflow::objectClassDropped, this, &ContentPanelWorkflow::onObjectClassDropped);
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

	libraryObjects::ObjectClass objectClass(metaObject);
//	auto metaEnum = QMetaEnum::fromType<types::ClassType::Type>();

	for (auto classType : objectClass.getClassType(types::ClassType::CONSTRUCTOR)) {
		if (classType->access() < minimumAccess) { continue; }

		objectClass.removeClassType(classType);
//		registry->registerModel<Method>(prefix + metaEnum.key(classType->type()), [=]() {
//			return std::unique_ptr<Method>(new Method(classType));
//		});
	}
	for (auto classType : objectClass.getClassType(types::ClassType::METHOD)) {
		if (classType->access() < minimumAccess) { continue; }

		objectClass.removeClassType(classType);
//		registry->registerModel<Method>(prefix + metaEnum.key(classType->type()), [=]() {
//			return std::unique_ptr<Method>(new Method(classType));
//		});
	}
	for (auto classType : objectClass.getClassType(types::ClassType::SIGNAL)) {
		if (classType->access() < minimumAccess) { continue; }

		objectClass.removeClassType(classType);
//		registry->registerModel<Method>(prefix + metaEnum.key(classType->type()), [=]() {
//			return std::unique_ptr<Method>(new Method(classType));
//		});
	}
	for (auto classType : objectClass.getClassType(types::ClassType::SLOT)) {
		if (classType->access() < minimumAccess) { continue; }

		objectClass.removeClassType(classType);
//		registry->registerModel<Method>(prefix + metaEnum.key(classType->type()), [=]() {
//			return std::unique_ptr<Method>(new Method(classType));
//		});
	}
	for (auto classType : objectClass.getClassType(types::ClassType::PROPERTY)) {
		if (classType->access() < minimumAccess) { continue; }

		objectClass.removeClassType(classType);
//		registry->registerModel<Property>(prefix + metaEnum.key(classType->type()), [=]() {
//			return std::unique_ptr<Method>(new Method(classType));
//		});
	}
	return registry;
}

void qtengine::ContentPanelWorkflow::onObjectChanged(libraryObjects::AObject *object)
{
	_tree->setObject(object);
	_tree->clear();
	_scene->clearScene();
	if (!object) { return; }

	auto registryView = generateRegistryView(object->object()->metaObject(), QMetaMethod::Protected);
	auto registry = std::make_shared<QtNodes::DataModelRegistry>();

	registry->concatenate(registryView.get());
	registry->concatenate(_registryBuiltIn.get());
	_scene->setRegistry(registry);
}

void qtengine::ContentPanelWorkflow::onObjectClassChanged(libraryObjects::ObjectClass *objectClass)
{
	_tree->setObjectClass(objectClass);
}

void qtengine::ContentPanelWorkflow::onObjectClassDropped(const QPointF &pos, libraryObjects::ObjectClass *objectClass, libraryObjects::AObject *reference)
{
	Q_UNUSED(objectClass)
	Q_UNUSED(reference)

	_view->openMenu(_view->mapFromScene(pos));
}
