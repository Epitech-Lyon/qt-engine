/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EStackedWidget
*/

#include "EStackedWidget.hpp"
#include "EWidget.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"
#include "ObjectManager.hpp"

#include <QtWidgets/QWidget>

template<> void libraryObjects::EStackedWidget::init(AObject *object)
{
	EWidget::init(object);
}

template<> QString libraryObjects::EStackedWidget::classIncludePath()
{
	return "#include <QtWidgets/QStackedWidget>";
}

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

template<> QString libraryObjects::EStackedWidget::code(AObject *object)
{
	QString code = EWidget::code(object);

	for (auto child : object->children())
		code += ObjectManager::instance()->objectName(object->id()) + "->addWidget(" + ObjectManager::instance()->objectName(child->id()) + ");\n";
	return code;
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
