/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EScrollArea
*/

#include "EScrollArea.hpp"
#include "EAbstractScrollArea.hpp"

#include "LibraryFunction.hpp"
#include "LibraryObjectManager.hpp"
#include "ObjectManager.hpp"

#include <QtWidgets/QWidget>

template<> void libraryObjects::EScrollArea::init(AObject *object)
{
	EAbstractScrollArea::init(object);
}

template<> QString libraryObjects::EScrollArea::classIncludePath()
{
	return "#include <QtWidgets/QScrollArea>";
}

template<> QIcon libraryObjects::EScrollArea::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EScrollArea::libraryFunction()
{
	auto libraryFunction = EAbstractScrollArea::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QWidget>::classHierarchy(), LibraryFunction::FunctionDrag("setWidget", ScrollArea::setWidget, "unsetWidget", ScrollArea::unsetWidget));
	return libraryFunction;
}

template<> QString libraryObjects::EScrollArea::code(AObject *object)
{
	QString code = EAbstractScrollArea::code(object);

	if (object->children().size() == 1 && LibraryObjectManager::isSubClassOf(object->children().front()->classHierarchy(), Object<QWidget>::classHierarchy()))
		code += ObjectManager::instance()->objectName(object->id()) + "->setWidget(" + ObjectManager::instance()->objectName(object->children().front()->id()) + ");\n";
	return code;
}

bool libraryObjects::ScrollArea::setWidget(AObject *parent, int, AObject *child)
{
	auto scrollArea = dynamic_cast<QScrollArea*>(parent->object());
	if (!scrollArea || scrollArea->widget()) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	scrollArea->setWidget(widget);
	parent->addChild(child);
	return true;
}

bool libraryObjects::ScrollArea::unsetWidget(AObject *parent, AObject *child)
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
