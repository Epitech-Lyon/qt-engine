/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewProperty
*/

#include "ContentPanelViewProperty.hpp"

qtengine::ContentPanelViewProperty::ContentPanelViewProperty(QWidget *parent)
	: ContentPanelBase("View Property", parent)
{
}

void qtengine::ContentPanelViewProperty::init()
{
	_propertyManager = new QtVariantPropertyManager(this);
	_propertyFactory = new QtVariantEditorFactory(this);
	_propertyEditor = new QtTreePropertyBrowser(this);
	_propertyEditor->setFactoryForManager(_propertyManager, _propertyFactory);
	_propertyEditor->setPropertiesWithoutValueMarked(true);
	_propertyEditor->setRootIsDecorated(false);
	_mainLayout->addWidget(_propertyEditor);
}
