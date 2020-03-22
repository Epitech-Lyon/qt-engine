/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EMenu
*/

#include "EMenu.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"

#include <QtWidgets/QAction>

template<> void libraryObjects::EMenu::init(AObject *object)
{
	EObject::init(object);
}

template<> QString libraryObjects::EMenu::classIncludePath()
{
	return "#include <QtWidgets/QMenu>";
}

template<> QIcon libraryObjects::EMenu::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EMenu::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QAction>::classHierarchy(), LibraryFunction::FunctionDrag("insertAction", Menu::insertAction, "removeAction", Menu::removeAction));
	libraryFunction->addFunctionDrag(Object<QMenu>::classHierarchy(), LibraryFunction::FunctionDrag("insertMenu", Menu::insertMenu, "removeMenu", Menu::removeMenu));
	return libraryFunction;
}

bool libraryObjects::Menu::insertAction(AObject *parent, int index, AObject *child)
{
	auto menu = dynamic_cast<QMenu*>(parent->object());
	if (!menu) { return false; }

	auto action = dynamic_cast<QAction*>(child->object());
	if (!action) { return false; }

	menu->insertAction(menu->actions().at(index), action);
	parent->insertChild(index, child);
	return true;
}

bool libraryObjects::Menu::removeAction(AObject *parent, AObject *child)
{
	auto menu = dynamic_cast<QMenu*>(parent->object());
	if (!menu) { return false; }

	auto action = dynamic_cast<QAction*>(child->object());
	if (!action) { return false; }

	menu->removeAction(action);
	parent->removeChild(child);
	return true;
}

bool libraryObjects::Menu::insertMenu(AObject *parent, int index, AObject *child)
{
	auto menu = dynamic_cast<QMenu*>(parent->object());
	if (!menu) { return false; }

	auto subMenu = dynamic_cast<QMenu*>(child->object());
	if (!subMenu) { return false; }

	menu->insertMenu(menu->actions().at(index), subMenu);
	parent->insertChild(index, child);
	return true;
}

bool libraryObjects::Menu::removeMenu(AObject *parent, AObject *child)
{
	auto menu = dynamic_cast<QMenu*>(parent->object());
	if (!menu) { return false; }

	auto subMenu = dynamic_cast<QMenu*>(child->object());
	if (!subMenu) { return false; }

	menu->removeAction(subMenu->menuAction());
	parent->removeChild(child);
	return true;
}