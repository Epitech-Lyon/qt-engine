/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EScrollArea
*/

#include "EScrollArea.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"

#include <QtWidgets/QWidget>

template<> QIcon libraryObjects::EScrollArea::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EScrollArea::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QWidget>::classHierarchy(), LibraryFunction::FunctionDrag("setWidget", setWidget, "unsetWidget", unsetWidget));
	return libraryFunction;
}

bool libraryObjects::setWidget(AObject *parent, int, AObject *child)
{
	auto scrollArea = dynamic_cast<QScrollArea*>(parent->object());
	if (!scrollArea || scrollArea->widget()) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	scrollArea->setWidget(widget);
	parent->addChild(child);
	return true;
}

bool libraryObjects::unsetWidget(AObject *parent, AObject *child)
{
	auto scrollArea = dynamic_cast<QScrollArea*>(parent->object());
	if (!scrollArea) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (scrollArea->widget() != widget) { return false; }

	scrollArea->setWidget(nullptr);
	parent->removeChild(child);
	return true;
}
