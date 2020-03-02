/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ETableView
*/

#include "ETableView.hpp"
#include "EAbstractItemView.hpp"

#include "LibraryFunction.hpp"

template<> void libraryObjects::ETableView::init(AObject *object)
{
	EAbstractItemView::init(object);
}

template<> QIcon libraryObjects::ETableView::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ETableView::libraryFunction()
{
	auto libraryFunction = EAbstractItemView::libraryFunction();

	return libraryFunction;
}
