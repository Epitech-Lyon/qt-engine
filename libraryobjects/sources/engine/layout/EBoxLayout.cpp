/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EBoxLayout
*/

#include "EBoxLayout.hpp"

#include <QtWidgets/QWidget>

template<> QIcon libraryObjects::EBoxLayout::icon()
{
	return QIcon();
}

template<> QPair<QString, libraryObjects::LibraryObject::FunctionDrag> libraryObjects::EBoxLayout::functionDrag()
{
	return { Object<QWidget>::classHierarchy(), addWidget };
}

libraryObjects::AObject *libraryObjects::addWidget(AObject *parent, int index, LibraryObject *child)
{
	auto boxLayout = dynamic_cast<QBoxLayout*>(parent->object());
	if (!boxLayout) { return nullptr; }

	auto childObject = child->constructor();
	auto widget = dynamic_cast<QWidget*>(childObject->object());

	boxLayout->insertWidget(index, widget);
	parent->addChild(childObject);
	return childObject;
}
