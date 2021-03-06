/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelObjectProperty
*/

#include "ContentPanelObjectProperty.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "AObject.hpp"

#include <QtCore/QDebug>
#include <QtCore/QMetaEnum>

qtengine::ContentPanelObjectProperty::ContentPanelObjectProperty(QWidget *parent)
	: ContentPanelBase("Object Property", parent)
{
}

void qtengine::ContentPanelObjectProperty::init()
{
	_propertyManager = new QtVariantPropertyManager(this);
	_propertyFactory = new QtVariantEditorFactory(this);
	_propertyEditor = new QtTreePropertyBrowser(this);

	_propertyEditor->setFactoryForManager(_propertyManager, _propertyFactory);
	_propertyEditor->setPropertiesWithoutValueMarked(true);
	_propertyEditor->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

	_mainLayout->addWidget(_propertyEditor);

	_init = false;
	_currentObject = nullptr;
	onCurrentObjectChanged(Manager::instance()->viewManager()->currentObject());
	connect(Manager::instance()->viewManager(), &ViewManager::currentObjectChanged, this, &ContentPanelObjectProperty::onCurrentObjectChanged);

	connect(_propertyManager, &QtVariantPropertyManager::valueChanged, this, &ContentPanelObjectProperty::onValueChanged);
}

void qtengine::ContentPanelObjectProperty::onCurrentObjectChanged(libraryObjects::AObject *object)
{
	_init = true;
	if (_currentObject != object) {
		if (_currentObject)
			disconnect(_currentObject, &libraryObjects::AObject::propertyUpdated, this, &ContentPanelObjectProperty::onPropertyUpdated);
		_currentObject = object;
		_propertyManager->clear();
		if (_currentObject) {
			initObject();
			connect(_currentObject, &libraryObjects::AObject::propertyUpdated, this, &ContentPanelObjectProperty::onPropertyUpdated);
		}
	} else if (_currentObject)
		refreshObject();
	_init = false;
}

void qtengine::ContentPanelObjectProperty::initObject()
{
	for (auto className : _currentObject->classHierarchy().split("::")) {
		auto propertyGroup = _propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), className);

		for (auto classProperty : _currentObject->properties(className)) {
			auto propertyName = classProperty.name;
			auto property = _currentObject->propertyValue(propertyName);
			auto objectProperty = 
				classProperty.isFlag ? initFlagProperty(propertyName, classProperty.keys) :
				classProperty.isEnum ? initEnumProperty(propertyName, classProperty.keys) :
				_propertyManager->addProperty(property.type(), propertyName);

			if (objectProperty) {
				objectProperty->setValue(property);
				propertyGroup->addSubProperty(objectProperty);
			} else
				qWarning() << "No editor for the property" << propertyName << "of type" << property.type();
		}
		if (propertyGroup->subProperties().size() > 0)
			_propertyEditor->addProperty(propertyGroup);
		else
			delete propertyGroup;
	}
	for (auto item : _propertyEditor->topLevelItems()) {
		for (auto subItem : item->children())
			_propertyEditor->setExpanded(subItem, false);
		_propertyEditor->setExpanded(item, false);
	}
}

QtVariantProperty *qtengine::ContentPanelObjectProperty::initFlagProperty(const QString &propertyName, const QStringList &flagNames)
{
	auto objectProperty = _propertyManager->addProperty(QtVariantPropertyManager::flagTypeId(), propertyName);

	objectProperty->setAttribute("flagNames", flagNames);
	return objectProperty;
}

QtVariantProperty *qtengine::ContentPanelObjectProperty::initEnumProperty(const QString &propertyName, const QStringList &enumNames)
{
	auto objectProperty = _propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), propertyName);

	objectProperty->setAttribute("enumNames", enumNames);
	return objectProperty;
}

void qtengine::ContentPanelObjectProperty::refreshObject()
{
	for (auto topLevelItem : _propertyEditor->topLevelItems())
		for (auto item : topLevelItem->property()->subProperties()) {
			auto variantProperty = dynamic_cast<QtVariantProperty*>(item);

			if (variantProperty)
				variantProperty->setValue(_currentObject->propertyValue(item->propertyName()));
		}
}

void qtengine::ContentPanelObjectProperty::onValueChanged(QtProperty *property, const QVariant &value)
{
	if (_init) { return; }
	_init = true;
	_currentObject->setPropertyValue(property->propertyName().toStdString().c_str(), value);
	_init = false;
}

void qtengine::ContentPanelObjectProperty::onPropertyUpdated(const QString &propertyName, const QVariant &propertyValue)
{
	_init = true;
	for (auto topLevelItem : _propertyEditor->topLevelItems())
		for (auto item : topLevelItem->property()->subProperties())
			if (item->propertyName() == propertyName) {
				auto variantProperty = dynamic_cast<QtVariantProperty*>(item);

				if (variantProperty)
					variantProperty->setValue(propertyValue);
			}
	_init = false;
}
