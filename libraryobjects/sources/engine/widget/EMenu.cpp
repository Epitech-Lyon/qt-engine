/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EMenu
*/

#include "EMenu.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"
#include "LibraryObjectManager.hpp"
#include "ObjectManager.hpp"

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

template<> QString libraryObjects::EMenu::code(AObject *object)
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

bool libraryObjects::Menu::insertAction(AObject *parent, int index, AObject *child)
{
	auto menu = dynamic_cast<QMenu*>(parent->object());
	if (!menu) { return false; }

	auto action = dynamic_cast<QAction*>(child->object());
	if (!action) { return false; }

	if (index >= 0 && index < menu->actions().size())
		menu->insertAction(menu->actions().at(index), action);
	else
		menu->addAction(action);
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

	if (index >= 0 && index < menu->actions().size())
		menu->insertMenu(menu->actions().at(index), subMenu);
	else
		menu->addMenu(subMenu);
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
