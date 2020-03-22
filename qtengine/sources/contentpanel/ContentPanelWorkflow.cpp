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
#include "ClassTypeManager.hpp"
#include "types/includes/Constructor.hpp"
#include "types/includes/Method.hpp"
#include "types/includes/Signal.hpp"
#include "types/includes/Slot.hpp"
#include "types/includes/Property.hpp"

#include "Utils.hpp"

#include "Start.hpp"
#include "Return.hpp"
#include "If.hpp"
#include "While.hpp"
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

	_currentClassType = nullptr;

	splitter->addWidget(_tree);
	splitter->addWidget(_view);
	splitter->setSizes({_tree->sizeHint().width(), _tree->sizeHint().width() * 4});

	connect(_scene, &FlowSceneWorkflow::objectClassDropped, this, &ContentPanelWorkflow::onObjectClassDropped);
	connect(_tree, &TreeWidgetWorkflow::classTypeDoubleClicked, this, &ContentPanelWorkflow::onClassTypeDoubleClicked);
	connect(_tree, &TreeWidgetWorkflow::classTypeDeleted, this, &ContentPanelWorkflow::onClassTypeDeleted);

	onObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelWorkflow::onObjectChanged);

	onObjectClassChanged(Manager::instance()->viewManager()->viewObjectClass());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectClassChanged, this, &ContentPanelWorkflow::onObjectClassChanged);

	connect(Manager::instance()->viewManager(), &ViewManager::saveRequested, this, &ContentPanelWorkflow::onSaveRequested);
}

std::shared_ptr<QtNodes::DataModelRegistry> qtengine::ContentPanelWorkflow::generateRegistryBuiltIn() const
{
	auto registry = std::make_shared<QtNodes::DataModelRegistry>();

	for (auto type : types::ClassTypeManager::instance()->types())
		if (!types::ClassTypeManager::instance()->isCustomType(type))
			registry->registerModel<BuiltIn>("Built-in", [type]() {
				auto modelBuiltIn = std::unique_ptr<BuiltIn>(new BuiltIn());

				modelBuiltIn->setData(types::ClassTypeManager::instance()->typeValue(type));
				return modelBuiltIn;
			});
	return registry;
}

std::shared_ptr<QtNodes::DataModelRegistry> qtengine::ContentPanelWorkflow::generateRegistryObjectClass(libraryObjects::ObjectClass *objectClass, QMetaMethod::Access minimumAccess, const QUuid &objectId) const
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
		if (!constructor || constructor->access() < minimumAccess || !objectId.isNull()) { continue; }

		auto constructorSave = constructor->serialize();
		registry->registerModel<Constructor>(metaEnumKeyOf(classType->type()), [constructorSave]() {
			auto modelConstructor = std::unique_ptr<Constructor>(new Constructor());

			modelConstructor->setData(constructorSave);
			return modelConstructor;
		});
	}
	for (auto classType : objectClass->getClassType(types::ClassType::METHOD)) {
		auto method = dynamic_cast<types::Method *>(classType);
		if (!method || method->access() < minimumAccess || (objectId.isNull() && !method->isStatic())) { continue; }

		auto methodSave = method->serialize();
		registry->registerModel<Method>(metaEnumKeyOf(classType->type()), [methodSave, objectId]() {
			auto modelMethod = std::unique_ptr<Method>(new Method());

			modelMethod->setData(methodSave, objectId);
			return modelMethod;
		});
	}
	for (auto classType : objectClass->getClassType(types::ClassType::SIGNAL)) {
		auto signal = dynamic_cast<types::Signal *>(classType);
		if (!signal || signal->access() < minimumAccess || objectId.isNull()) { continue; }

		auto signalSave = signal->serialize();
		registry->registerModel<Signal>(metaEnumKeyOf(classType->type()), [signalSave, objectId]() {
			auto modelSignal = std::unique_ptr<Signal>(new Signal());

			modelSignal->setData(signalSave, objectId);
			return modelSignal;
		});
	}
	for (auto classType : objectClass->getClassType(types::ClassType::SLOT)) {
		auto slot = dynamic_cast<types::Slot *>(classType);
		if (!slot || slot->access() < minimumAccess || objectId.isNull()) { continue; }

		auto slotSave = slot->serialize();
		registry->registerModel<Slot>(metaEnumKeyOf(classType->type()), [slotSave, objectId]() {
			auto modelSlot = std::unique_ptr<Slot>(new Slot());

			modelSlot->setData(slotSave, objectId);
			return modelSlot;
		});
	}
	for (auto classType : objectClass->getClassType(types::ClassType::PROPERTY)) {
		auto property = dynamic_cast<types::Property *>(classType);
		if (!property || objectId.isNull()) { continue; }

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
	_currentClassType = nullptr;
	_tree->setObject(object);
	_tree->clear();
	_scene->clearScene();
	if (!object) { return; }

	libraryObjects::ObjectClass objectClass(object->object()->metaObject());

	auto registry = generateRegistryObjectClass(&objectClass, QMetaMethod::Protected, object->id());
	registry->addPrefix("base class/");
	registry->concatenate(generateRegistryBuiltIn().get());
	registry->registerModel<If>();
	registry->registerModel<While>();

	_scene->setRegistry(registry);
}

void qtengine::ContentPanelWorkflow::onObjectClassChanged(libraryObjects::ObjectClass *objectClass)
{
	_tree->setObjectClass(objectClass);
}

void qtengine::ContentPanelWorkflow::onObjectClassDropped(const QPointF &pos, libraryObjects::ObjectClass *objectClass, libraryObjects::AObject *reference, QObject *source)
{
	auto minimumAccess = source == _tree ? QMetaMethod::Private : reference == _tree->object() ? QMetaMethod::Protected : QMetaMethod::Public;
	auto tmpRegistry = generateRegistryObjectClass(objectClass, minimumAccess, reference ? reference->id() : "");
	auto saveRegistry = _scene->takeRegistry();

	_scene->setRegistry(tmpRegistry);
	_view->openMenu(_view->mapFromScene(pos), true);
	_scene->setRegistry(saveRegistry);
}

void qtengine::ContentPanelWorkflow::onClassTypeDoubleClicked(types::ClassType *classType)
{
	if (classType->type() == types::ClassType::SIGNAL || classType->type() == types::ClassType::PROPERTY) { return; }

	onSaveRequested();
	_currentClassType = classType;

	auto tmpRegistry = std::make_shared<QtNodes::DataModelRegistry>();
	tmpRegistry->registerModel<Start>([classType]() {
		if (classType->type() == types::ClassType::METHOD) {
			auto method = dynamic_cast<types::Method*>(classType);
			return std::unique_ptr<Start>(new Start(method->parameters()));
		} else if (classType->type() == types::ClassType::SLOT) {
			auto slot = dynamic_cast<types::Slot*>(classType);
			return std::unique_ptr<Start>(new Start(slot->parameters()));
		} else if (classType->type() == types::ClassType::CONSTRUCTOR) {
			auto constructor = dynamic_cast<types::Constructor*>(classType);
			return std::unique_ptr<Start>(new Start(constructor->parameters()));
		}
		return std::unique_ptr<Start>();
	});
	tmpRegistry->registerModel<Return>([classType]() {
		if (classType->type() == types::ClassType::METHOD) {
			auto method = dynamic_cast<types::Method*>(classType);
			return std::unique_ptr<Return>(new Return(method->returnType()));
		} else {
			return std::unique_ptr<Return>(new Return(types::ClassTypeManager::instance()->type(QMetaType::Void)));
		}
	});
	tmpRegistry->registerModel<If>();
	tmpRegistry->registerModel<While>();
	tmpRegistry->registerModel<BuiltIn>();
	tmpRegistry->registerModel<Constructor>();
	tmpRegistry->registerModel<Method>();
	tmpRegistry->registerModel<Signal>();
	tmpRegistry->registerModel<Slot>();
	tmpRegistry->registerModel<Property>();
	auto saveRegistry = _scene->takeRegistry();

	_scene->clearScene();
	_scene->setRegistry(tmpRegistry);
	_scene->loadFromJson(_currentClassType->content());

	QString startName = Start({}).name();
	bool startFound = false;
	_scene->iterateOverNodeData([startName, &startFound](QtNodes::NodeDataModel *model) {
		startFound = !startFound ? model->name() == startName : startFound;
	});
	if (!startFound)
		_scene->createNode(startName, QPointF(-100, 0));

	saveRegistry->unregisterModel(Return("").name());
	saveRegistry->registerModel<Return>([classType]() {
		if (classType->type() == types::ClassType::METHOD) {
			auto method = dynamic_cast<types::Method*>(classType);
			return std::unique_ptr<Return>(new Return(method->returnType()));
		} else {
			return std::unique_ptr<Return>(new Return(types::ClassTypeManager::instance()->type(QMetaType::Void)));
		}
	});

	_scene->setRegistry(saveRegistry);
}

void qtengine::ContentPanelWorkflow::onClassTypeDeleted(types::ClassType *classType)
{
	if (classType != _currentClassType) { return; }

	_currentClassType = nullptr;
	_tree->selectDefaultConstructor();
}

void qtengine::ContentPanelWorkflow::onSaveRequested()
{
	if (!_currentClassType) { return; }

	_currentClassType->setContent(_scene->saveToJson());
}
