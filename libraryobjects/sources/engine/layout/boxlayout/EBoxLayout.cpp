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

template<> void libraryObjects::EBoxLayout::init(AObject *object)
{
	ELayout::init(object);
}

template<> libraryObjects::LibraryFunction *libraryObjects::EBoxLayout::libraryFunction()
{
	auto libraryFunction = ELayout::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QWidget>::classHierarchy(), LibraryFunction::FunctionDrag("insertWidget", BoxLayout::insertWidget, "removeWidget", Layout::removeWidget));
	libraryFunction->addFunctionDrag(Object<QLayout>::classHierarchy(), LibraryFunction::FunctionDrag("insertLayout", BoxLayout::insertLayout, "removeLayout", Layout::removeLayout));
	return libraryFunction;
}

bool libraryObjects::BoxLayout::insertWidget(AObject *parent, int index, AObject *child)
{
	auto boxLayout = dynamic_cast<QBoxLayout*>(parent->object());
	if (!boxLayout) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	boxLayout->insertWidget(index, widget);
	parent->insertChild(index, child);
	return true;
}

bool libraryObjects::BoxLayout::insertLayout(AObject *parent, int index, AObject *child)
{
	auto boxLayout = dynamic_cast<QBoxLayout*>(parent->object());
	if (!boxLayout) { return false; }

	auto layout = dynamic_cast<QLayout*>(child->object());
	if (!layout) { return false; }

	boxLayout->insertLayout(index, layout);
	parent->insertChild(index, child);
	return true;
}
