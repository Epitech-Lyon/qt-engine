/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EStackedWidget
*/

#include "EStackedWidget.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"

#include <QtWidgets/QWidget>

template<> QIcon libraryObjects::EStackedWidget::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EStackedWidget::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QWidget>::classHierarchy(), LibraryFunction::FunctionDrag("insertWidget", StackedWidget::insertWidget, "removeWidget", StackedWidget::removeWidget));
	return libraryFunction;
}

bool libraryObjects::StackedWidget::insertWidget(AObject *parent, int index, AObject *child)
{
	auto stackedWidget = dynamic_cast<QStackedWidget*>(parent->object());
	if (!stackedWidget) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (stackedWidget->indexOf(widget) != -1) { return false; }

	stackedWidget->insertWidget(index, widget);
	parent->insertChild(index, child);
	return true;
}

bool libraryObjects::StackedWidget::removeWidget(AObject *parent, AObject *child)
{
	auto stackedWidget = dynamic_cast<QStackedWidget*>(parent->object());
	if (!stackedWidget) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (stackedWidget->indexOf(widget) == -1) { return false; }

	stackedWidget->removeWidget(widget);
	parent->removeChild(child);
	return true;
}
