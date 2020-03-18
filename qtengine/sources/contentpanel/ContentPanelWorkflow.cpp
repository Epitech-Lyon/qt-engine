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
#include "types/includes/Constructor.hpp"
#include "types/includes/Method.hpp"
#include "types/includes/Signal.hpp"
#include "types/includes/Slot.hpp"
#include "types/includes/Property.hpp"

#include "Utils.hpp"

#include "Constructor.hpp"
#include "Method.hpp"
#include "Signal.hpp"
#include "Slot.hpp"
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

std::shared_ptr<QtNodes::DataModelRegistry> qtengine::ContentPanelWorkflow::generateRegistryObjectClass(libraryObjects::ObjectClass *objectClass, QMetaMethod::Access minimumAccess, const QString &objectId) const
{
	auto registry = std::make_shared<QtNodes::DataModelRegistry>();

	auto metaEnum = QMetaEnum::fromType<types::ClassType::Type>();
	auto metaEnumKeyOf = [metaEnum](types::ClassType::Type type) {
		QString name = metaEnum.key(type);
		name = name.toLower();

		QCharRef firstLetter = name.front();
		firstLetter = firstLetter.toUpper();
		return name;
	};

	for (auto classType : objectClass->getClassType(types::ClassType::CONSTRUCTOR)) {
		auto constructor = dynamic_cast<types::Constructor *>(classType);
		if (!constructor || constructor->access() < minimumAccess || !objectId.isEmpty()) { continue; }

		auto constructorSave = constructor->serialize();
		registry->registerModel<Constructor>(metaEnumKeyOf(classType->type()), [constructorSave]() {
			auto modelConstructor = std::unique_ptr<Constructor>(new Constructor());

			modelConstructor->setData(constructorSave);
			return modelConstructor;
		});
	}
	for (auto classType : objectClass->getClassType(types::ClassType::METHOD)) {
		auto method = dynamic_cast<types::Method *>(classType);
		if (!method || method->access() < minimumAccess || (objectId.isEmpty() && !method->isStatic())) { continue; }

		auto methodSave = method->serialize();
		registry->registerModel<Method>(metaEnumKeyOf(classType->type()), [methodSave, objectId]() {
			auto modelMethod = std::unique_ptr<Method>(new Method());

			modelMethod->setData(methodSave, objectId);
			return modelMethod;
		});
	}
	for (auto classType : objectClass->getClassType(types::ClassType::SIGNAL)) {
		auto signal = dynamic_cast<types::Signal *>(classType);
		if (!signal || signal->access() < minimumAccess || objectId.isEmpty()) { continue; }

		auto signalSave = signal->serialize();
		registry->registerModel<Signal>(metaEnumKeyOf(classType->type()), [signalSave, objectId]() {
			auto modelSignal = std::unique_ptr<Signal>(new Signal());

			modelSignal->setData(signalSave, objectId);
			return modelSignal;
		});
	}
	for (auto classType : objectClass->getClassType(types::ClassType::SLOT)) {
		auto slot = dynamic_cast<types::Slot *>(classType);
		if (!slot || slot->access() < minimumAccess || objectId.isEmpty()) { continue; }

		auto slotSave = slot->serialize();
		registry->registerModel<Slot>(metaEnumKeyOf(classType->type()), [slotSave, objectId]() {
			auto modelSlot = std::unique_ptr<Slot>(new Slot());

			modelSlot->setData(slotSave, objectId);
			return modelSlot;
		});
	}
	for (auto classType : objectClass->getClassType(types::ClassType::PROPERTY)) {
		auto property = dynamic_cast<types::Property *>(classType);
		if (!property || property->access() < minimumAccess || objectId.isEmpty()) { continue; }

		auto propertySave = property->serialize();
		registry->registerModel<Method>(metaEnumKeyOf(classType->type()), [propertySave, objectId]() {
			auto modelProperty = std::unique_ptr<Property>(new Property());

			modelProperty->setData(propertySave, objectId);
			return modelProperty;
		});
	}
	return registry;
}

void qtengine::ContentPanelWorkflow::onObjectChanged(libraryObjects::AObject *object)
{
	_tree->setObject(object);
	_tree->clear();
	_scene->clearScene();
	if (!object) { return; }

	libraryObjects::ObjectClass objectClass(object->object()->metaObject());

	auto registry = generateRegistryObjectClass(&objectClass, QMetaMethod::Protected, object->id());
	registry->addPrefix("base class/");
	registry->concatenate(generateRegistryBuiltIn().get());

	_scene->setRegistry(registry);
}

void qtengine::ContentPanelWorkflow::onObjectClassChanged(libraryObjects::ObjectClass *objectClass)
{
	_tree->setObjectClass(objectClass);
}

void qtengine::ContentPanelWorkflow::onObjectClassDropped(const QPointF &pos, libraryObjects::ObjectClass *objectClass, libraryObjects::AObject *reference)
{
	auto saveRegistry = _scene->takeRegistry();

	auto minimumAccess = reference == _tree->object() ? QMetaMethod::Private : QMetaMethod::Public;
	_scene->setRegistry(generateRegistryObjectClass(objectClass, minimumAccess, reference ? reference->id() : ""));

	_view->openMenu(_view->mapFromScene(pos), true);
	_scene->setRegistry(saveRegistry);
}
