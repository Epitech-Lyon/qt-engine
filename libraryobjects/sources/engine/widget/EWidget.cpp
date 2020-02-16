/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EWidget
*/

#include "EWidget.hpp"

#include <QtWidgets/QLayout>

template<> QIcon libraryObjects::EWidget::icon()
{
	return QIcon();
}

template<> QPair<QString, libraryObjects::LibraryObject::FunctionDrag> libraryObjects::EWidget::functionDrag()
{
	return {Object<QLayout>::classHierarchy(), setLayout};
}

libraryObjects::AObject *libraryObjects::setLayout(AObject *parent, int, LibraryObject *child)
{
	auto widget = dynamic_cast<QWidget*>(parent->object());
	if (!widget || widget->layout()) { return nullptr; }

	auto childObject = child->constructor();
	auto layout = dynamic_cast<QLayout*>(childObject->object());

	widget->setLayout(layout);
	parent->addChild(childObject);
	return childObject;
}
