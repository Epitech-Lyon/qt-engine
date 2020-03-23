/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EToolButton
*/

#include "EToolButton.hpp"
#include "EAbstractButton.hpp"

#include "LibraryFunction.hpp"
#include "LibraryObjectManager.hpp"
#include "ObjectManager.hpp"

#include <QtWidgets/QMenu>

template<> void libraryObjects::EToolButton::init(AObject *object)
{
	EAbstractButton::init(object);
}

template<> QString libraryObjects::EToolButton::classIncludePath()
{
	return "#include <QtWidgets/QToolButton>";
}

template<> QIcon libraryObjects::EToolButton::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EToolButton::libraryFunction()
{
	auto libraryFunction = EAbstractButton::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QMenu>::classHierarchy(), LibraryFunction::FunctionDrag("setMenu", ToolButton::setMenu, "unsetMenu", ToolButton::unsetMenu));
	return libraryFunction;
}

template<> QString libraryObjects::EToolButton::code(AObject *object)
{
	QString code = EAbstractButton::code(object);

	if (object->children().size() == 1 && LibraryObjectManager::isSubClassOf(object->children().front()->classHierarchy(), Object<QMenu>::classHierarchy()))
		code += ObjectManager::instance()->objectName(object->id()) + "->setMenu(" + ObjectManager::instance()->objectName(object->children().front()->id()) + ");\n";
	return code;
}

bool libraryObjects::ToolButton::setMenu(AObject *parent, int, AObject *child)
{
	auto toolButton = dynamic_cast<QToolButton*>(parent->object());
	if (!toolButton || toolButton->menu()) { return false; }

	auto menu = dynamic_cast<QMenu*>(child->object());
	if (!menu) { return false; }

	toolButton->setMenu(menu);
	parent->addChild(child);
	return true;
}

bool libraryObjects::ToolButton::unsetMenu(AObject *parent, AObject *child)
{
	auto toolButton = dynamic_cast<QToolButton*>(parent->object());
	if (!toolButton) { return false; }

	auto menu = dynamic_cast<QMenu*>(child->object());
	if (!menu) { return false; }

	toolButton->setMenu(nullptr);
	parent->removeChild(child);
	return true;
}
