/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ESplitter
*/

#include "ESplitter.hpp"
#include "EWidget.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"
#include <QtCore/QJsonArray>

#include <QtWidgets/QWidget>

template<> void libraryObjects::ESplitter::init(AObject *object)
{
	EWidget::init(object);
}

template<> QJsonObject libraryObjects::ESplitter::serializeData(AObject *object)
{
	auto splitter = dynamic_cast<QSplitter*>(object->object());
	if (!splitter) { return QJsonObject(); }

	QJsonArray jsonSizes;
	for (auto size : splitter->sizes())
		jsonSizes.append(size);

	QJsonObject json;
	json["Sizes"] = jsonSizes;
	return json;
}

template<> void libraryObjects::ESplitter::deserializeData(const QJsonObject &json, AObject *object)
{
	auto splitter = dynamic_cast<QSplitter*>(object->object());
	if (!splitter) { return; }

	QList<int> sizes;
	for (auto sizeRef : json["Sizes"].toArray())
		sizes << sizeRef.toInt();

	splitter->setSizes(sizes);
}

template<> QString libraryObjects::ESplitter::classIncludePath()
{
	return "#include <QtWidgets/QSplitter>";
}

template<> QIcon libraryObjects::ESplitter::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ESplitter::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QWidget>::classHierarchy(), LibraryFunction::FunctionDrag("insertWidget", Splitter::insertWidget, "removeWidget", Splitter::removeWidget));
	return libraryFunction;
}

bool libraryObjects::Splitter::insertWidget(AObject *parent, int index, AObject *child)
{
	auto splitter = dynamic_cast<QSplitter*>(parent->object());
	if (!splitter) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (splitter->indexOf(widget) != -1) { return false; }

	splitter->insertWidget(index, widget);
	parent->insertChild(index, child);
	return true;
}

bool libraryObjects::Splitter::removeWidget(AObject *parent, AObject *child)
{
	auto splitter = dynamic_cast<QSplitter*>(parent->object());
	if (!splitter) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (splitter->indexOf(widget) == -1) { return false; }

	parent->removeChild(child);
	return true;
}
