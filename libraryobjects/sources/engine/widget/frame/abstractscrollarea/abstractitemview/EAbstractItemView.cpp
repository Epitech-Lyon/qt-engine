/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EAbstractItemView
*/

#include "EAbstractItemView.hpp"
#include "EAbstractScrollArea.hpp"

#include "LibraryFunction.hpp"

#include <QtCore/QAbstractItemModel>

template<> void libraryObjects::EAbstractItemView::init(AObject *object)
{
	EAbstractScrollArea::init(object);
}

template<> libraryObjects::LibraryFunction *libraryObjects::EAbstractItemView::libraryFunction()
{
	auto libraryFunction = EAbstractScrollArea::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QAbstractItemModel>::classHierarchy(), LibraryFunction::FunctionDrag("setModel", AbstractItemView::setModel, "unsetModel", AbstractItemView::unsetModel));
	return libraryFunction;
}

bool libraryObjects::AbstractItemView::setModel(AObject *parent, int, AObject *child)
{
	auto abstractItemView = dynamic_cast<QAbstractItemView*>(parent->object());
	if (!abstractItemView || abstractItemView->model()) { return false; }

	auto abstractItemModel = dynamic_cast<QAbstractItemModel*>(child->object());
	if (!abstractItemModel) { return false; }

	abstractItemView->setModel(abstractItemModel);
	parent->addChild(child);
	return true;
}

bool libraryObjects::AbstractItemView::unsetModel(AObject *parent, AObject *child)
{
	auto abstractItemView = dynamic_cast<QAbstractItemView*>(parent->object());
	if (!abstractItemView) { return false; }

	auto abstractItemModel = dynamic_cast<QAbstractItemModel*>(child->object());
	if (!abstractItemModel) { return false; }

	if (abstractItemView->model() != abstractItemModel) { return false; }

	abstractItemView->setModel(nullptr);
	parent->removeChild(child);
	return true;
}
