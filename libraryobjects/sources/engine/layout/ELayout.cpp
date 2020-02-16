/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ELayout
*/

#include "ELayout.hpp"
#include "EObject.hpp"

#include <QtWidgets/QWidget>

template<> libraryObjects::LibraryFunction *libraryObjects::ELayout::libraryFunction()
{
	return EObject::libraryFunction();
}

bool libraryObjects::removeWidget(AObject *parent, AObject *child)
{
	auto layout = dynamic_cast<QLayout*>(parent->object());
	if (!layout) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (layout->indexOf(widget) == -1) { return false; }

	layout->removeWidget(widget);
	parent->removeChild(child);
	return true;
}

bool libraryObjects::removeLayout(AObject *parent, AObject *child)
{
	auto layout = dynamic_cast<QLayout*>(parent->object());
	if (!layout) { return false; }

	for (int i = 0; i < layout->count(); i += 1) {
		auto item = layout->itemAt(i);

		if (item && item->layout() == child->object()) {
			layout->removeItem(item);
			return true;
		}
	}
	return false;
}
