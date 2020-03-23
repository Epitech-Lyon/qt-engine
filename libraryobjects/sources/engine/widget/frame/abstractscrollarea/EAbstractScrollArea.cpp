/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EAbstractScrollArea
*/

#include "EAbstractScrollArea.hpp"
#include "EWidget.hpp"
#include "EObject.hpp"

template<> void libraryObjects::EAbstractScrollArea::init(AObject *object)
{
	EWidget::init(object);
}

template<> libraryObjects::LibraryFunction *libraryObjects::EAbstractScrollArea::libraryFunction()
{
	return EObject::libraryFunction();
}

template<> QString libraryObjects::EAbstractScrollArea::code(AObject *object)
{
	return EWidget::code(object);
}
