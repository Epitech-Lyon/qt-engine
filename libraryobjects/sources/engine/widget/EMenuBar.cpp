/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EMenuBar
*/

#include "EMenuBar.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"
#include "LibraryObjectManager.hpp"
#include "ObjectManager.hpp"

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>

template<> void libraryObjects::EMenuBar::init(AObject *object)
{
	EObject::init(object);
}

template<> QString libraryObjects::EMenuBar::classIncludePath()
{
	return "#include <QtWidgets/QMenuBar>";
}

template<> QIcon libraryObjects::EMenuBar::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EMenuBar::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QAction>::classHierarchy(), LibraryFunction::FunctionDrag("insertAction", MenuBar::insertAction, "removeAction", MenuBar::removeAction));
	libraryFunction->addFunctionDrag(Object<QMenu>::classHierarchy(), LibraryFunction::FunctionDrag("insertMenu", MenuBar::insertMenu, "removeMenu", MenuBar::removeMenu));
	return libraryFunction;
}

template<> QString libraryObjects::EMenuBar::code(AObject *object)
{
	QString code = EObject::code(object);

	for (auto children : object->children()) {
		if (LibraryObjectManager::isSubClassOf(children->classHierarchy(), Object<QAction>::classHierarchy()))
			code += ObjectManager::instance()->objectName(object->id()) + "->addAction(" + ObjectManager::instance()->objectName(children->id()) + ");\n";
		else if (LibraryObjectManager::isSubClassOf(children->classHierarchy(), Object<QMenu>::classHierarchy()))
			code += ObjectManager::instance()->objectName(object->id()) + "->addMenu(" + ObjectManager::instance()->objectName(children->id()) + ");\n";
	}
	return code;
}

bool libraryObjects::MenuBar::insertAction(AObject *parent, int index, AObject *child)
{
	auto menuBar = dynamic_cast<QMenuBar*>(parent->object());
	if (!menuBar) { return false; }

	auto action = dynamic_cast<QAction*>(child->object());
	if (!action) { return false; }

	if (index >= 0 && index < menuBar->actions().size())
		menuBar->insertAction(menuBar->actions().at(index), action);
	else
		menuBar->addAction(action);
	parent->insertChild(index, child);
	return true;
}

bool libraryObjects::MenuBar::removeAction(AObject *parent, AObject *child)
{
	auto menuBar = dynamic_cast<QMenuBar*>(parent->object());
	if (!menuBar) { return false; }

	auto action = dynamic_cast<QAction*>(child->object());
	if (!action) { return false; }

	menuBar->removeAction(action);
	parent->removeChild(child);
	return true;
}

bool libraryObjects::MenuBar::insertMenu(AObject *parent, int index, AObject *child)
{
	auto menuBar = dynamic_cast<QMenuBar*>(parent->object());
	if (!menuBar) { return false; }

	auto menu = dynamic_cast<QMenu*>(child->object());
	if (!menu) { return false; }

	if (index >= 0 && index < menuBar->actions().size())
		menuBar->insertMenu(menuBar->actions().at(index), menu);
	else
		menuBar->addMenu(menu);
	parent->insertChild(index, child);
	return true;
}

bool libraryObjects::MenuBar::removeMenu(AObject *parent, AObject *child)
{
	auto menuBar = dynamic_cast<QMenuBar*>(parent->object());
	if (!menuBar) { return false; }

	auto menu = dynamic_cast<QMenu*>(child->object());
	if (!menu) { return false; }

	menuBar->removeAction(menu->menuAction());
	parent->removeChild(child);
	return true;
}
