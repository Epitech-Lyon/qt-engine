/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EBoxLayout
*/

#include "EBoxLayout.hpp"
#include "ELayout.hpp"

#include "LibraryFunction.hpp"
#include "LibraryObjectManager.hpp"
#include "ObjectManager.hpp"

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

template<> QString libraryObjects::EBoxLayout::code(AObject *object)
{
	QString code = ELayout::code(object);

	for (auto children : object->children()) {
		if (LibraryObjectManager::isSubClassOf(children->classHierarchy(), Object<QWidget>::classHierarchy()))
			code += ObjectManager::instance()->objectName(object->id()) + "->addWidget(" + ObjectManager::instance()->objectName(children->id()) + ");\n";
		else if (LibraryObjectManager::isSubClassOf(children->classHierarchy(), Object<QLayout>::classHierarchy()))
			code += ObjectManager::instance()->objectName(object->id()) + "->addLayout(" + ObjectManager::instance()->objectName(children->id()) + ");\n";
	}
	return code;
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
