/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EToolBox
*/

#include "EToolBox.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"

#include <QtWidgets/QWidget>
#include <QtWidgets/QInputDialog>

template<> QIcon libraryObjects::EToolBox::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EToolBox::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QWidget>::classHierarchy(), LibraryFunction::FunctionDrag("insertWidget", ToolBox::insertItem, "removeWidget", ToolBox::removeItem));
	return libraryFunction;
}

bool libraryObjects::ToolBox::insertItem(AObject *parent, int index, AObject *child)
{
	auto toolBox = dynamic_cast<QToolBox*>(parent->object());
	if (!toolBox) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (toolBox->indexOf(widget) != -1) { return false; }

	bool ok = false;
	auto title = QInputDialog::getText(nullptr, "insert Item", "Please enter the title of the item", QLineEdit::Normal, "", &ok);
	if (!ok || title.isEmpty()) { return false; }

	toolBox->insertItem(index, widget, title);
	parent->insertChild(index, child);
	return true;
}

bool libraryObjects::ToolBox::removeItem(AObject *parent, AObject *child)
{
	auto toolBox = dynamic_cast<QToolBox*>(parent->object());
	if (!toolBox) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (toolBox->indexOf(widget) == -1) { return false; }

	toolBox->removeItem(toolBox->indexOf(widget));
	parent->removeChild(child);
	return true;
}
