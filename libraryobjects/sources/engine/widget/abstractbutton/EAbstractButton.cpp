/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EAbstractButton
*/

#include "EAbstractButton.hpp"
#include "EWidget.hpp"
#include "EObject.hpp"

template<> void libraryObjects::EAbstractButton::init(AObject *object)
{
	auto abstractButton = dynamic_cast<QAbstractButton*>(object->object());

	EObject::init(object);
	connect(abstractButton, &QAbstractButton::clicked, object, [object](bool checked) { emit object->propertyUpdated("checked", checked); });
}

template<> libraryObjects::LibraryFunction *libraryObjects::EAbstractButton::libraryFunction()
{
	return EObject::libraryFunction();
}

template<> QString libraryObjects::EAbstractButton::code(AObject *object)
{
	return EWidget::code(object);
}
