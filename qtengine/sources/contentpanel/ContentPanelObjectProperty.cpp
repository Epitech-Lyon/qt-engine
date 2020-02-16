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
		_currentObject = object;
		_propertyManager->clear();
		if (_currentObject)
			initObject();
	} else if (_currentObject)
		refreshObject();
	_init = false;
}

void qtengine::ContentPanelObjectProperty::initObject()
{
	for (auto className : _currentObject->classHierarchy().split("::")) {
		auto propertyGroup = _propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), className);

		for (auto classProperty : _currentObject->properties(className)) {
			auto property = _currentObject->propertyValue(classProperty.name);
			auto objectProperty = _propertyManager->addProperty(property.type(), classProperty.name);

			if (objectProperty) {
				objectProperty->setValue(property);
				propertyGroup->addSubProperty(objectProperty);
			} else
				qWarning() << "No editor for the property" << classProperty.name << "of type" << property.type();
		}
		if (propertyGroup->subProperties().size() > 0)
			_propertyEditor->addProperty(propertyGroup);
		else
			delete propertyGroup;
	}
	for (auto item : _propertyEditor->topLevelItems())
		_propertyEditor->setExpanded(item, false);
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
	_currentObject->setPropertyValue(property->propertyName().toStdString().c_str(), value);
	Manager::instance()->viewManager()->setCurrentObject(_currentObject);
}
