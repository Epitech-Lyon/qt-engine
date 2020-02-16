/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EWidget
*/

#include "EWidget.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"

#include <QtWidgets/QLayout>

template<> QIcon libraryObjects::EWidget::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EWidget::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QLayout>::classHierarchy(), LibraryFunction::FunctionDrag("setLayout", setLayout, "unsetLayout", unsetLayout));
	return libraryFunction;
}

bool libraryObjects::setLayout(AObject *parent, int, AObject *child)
{
	auto widget = dynamic_cast<QWidget*>(parent->object());
	if (!widget || widget->layout()) { return false; }

	auto layout = dynamic_cast<QLayout*>(child->object());
	if (!layout) { return false; }

	widget->setLayout(layout);
	parent->addChild(child);
	return true;
}

bool libraryObjects::unsetLayout(AObject *parent, AObject *child)
{
	auto widget = dynamic_cast<QWidget*>(parent->object());
	if (!widget || widget->layout()) { return false; }

	auto layout = dynamic_cast<QLayout*>(child->object());
	if (!layout) { return false; }

	widget->setLayout(nullptr);
	parent->removeChild(child);
	return true;
}
