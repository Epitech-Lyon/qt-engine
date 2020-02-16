/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EBoxLayout
*/

#include "EBoxLayout.hpp"
#include "ELayout.hpp"

#include "LibraryFunction.hpp"

#include <QtWidgets/QWidget>
#include <QtWidgets/QLayout>

template<> libraryObjects::LibraryFunction *libraryObjects::EBoxLayout::libraryFunction()
{
	auto libraryFunction = ELayout::libraryFunction();

	libraryFunction->addDragFunction(Object<QWidget>::classHierarchy(), LibraryFunction::FunctionDrag("insertWidget", insertWidget, "removeWidget", removeWidget));
	libraryFunction->addDragFunction(Object<QLayout>::classHierarchy(), LibraryFunction::FunctionDrag("insertLayout", insertLayout, "removeLayout", removeLayout));
	return libraryFunction;
}

bool libraryObjects::insertWidget(AObject *parent, int index, AObject *child)
{
	auto boxLayout = dynamic_cast<QBoxLayout*>(parent->object());
	if (!boxLayout) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	boxLayout->insertWidget(index, widget);
	parent->addChild(child);
	return true;
}

bool libraryObjects::insertLayout(AObject *parent, int index, AObject *child)
{
	auto boxLayout = dynamic_cast<QBoxLayout*>(parent->object());
	if (!boxLayout) { return false; }

	auto layout = dynamic_cast<QLayout*>(child->object());
	if (!layout) { return false; }

	boxLayout->insertLayout(index, layout);
	parent->addChild(child);
	return true;
}
