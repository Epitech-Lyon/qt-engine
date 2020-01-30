/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelObjectProperty
*/

#include "ContentPanelObjectProperty.hpp"

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
}
