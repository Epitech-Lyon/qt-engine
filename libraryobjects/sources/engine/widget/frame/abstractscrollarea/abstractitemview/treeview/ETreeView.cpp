/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ETreeView
*/

#include "ETreeView.hpp"
#include "EAbstractItemView.hpp"

#include "LibraryFunction.hpp"

template<> void libraryObjects::ETreeView::init(AObject *object)
{
	EAbstractItemView::init(object);
}

template<> QIcon libraryObjects::ETreeView::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ETreeView::libraryFunction()
{
	auto libraryFunction = EAbstractItemView::libraryFunction();

	return libraryFunction;
}
