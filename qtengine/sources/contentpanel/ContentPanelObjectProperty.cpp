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
	_propertyEditor->setRootIsDecorated(false);
	_mainLayout->addWidget(_propertyEditor);

	onCurrentObjectChanged(Manager::instance()->viewManager()->currentObject());
	connect(Manager::instance()->viewManager(), &ViewManager::currentObjectChanged, this, &ContentPanelObjectProperty::onCurrentObjectChanged);

	connect(_propertyManager, &QtVariantPropertyManager::valueChanged, this, &ContentPanelObjectProperty::onValueChanged);
}

void qtengine::ContentPanelObjectProperty::onCurrentObjectChanged(libraryObjects::AObject *object)
{
	_propertyManager->clear();
	if (!object) { return; }
	for (auto className : object->classHierarchy().split("::")) {
		auto propertyGroup = _propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), className);

		for (auto classProperty : object->properties(className)) {
			auto property = object->propertyValue(classProperty.name);
			auto objectProperty = _propertyManager->addProperty(property.type(), classProperty.name);

			if (objectProperty) {
				objectProperty->setValue(property);
				propertyGroup->addSubProperty(objectProperty);
			}
		}
		if (propertyGroup->subProperties().size() > 0)
			_propertyEditor->addProperty(propertyGroup);
		else
			delete propertyGroup;
	}
}

void qtengine::ContentPanelObjectProperty::onValueChanged(QtProperty *property, const QVariant &value)
{
	Manager::instance()->viewManager()->currentObject()->setPropertyValue(property->propertyName().toStdString().c_str(), value);
}
