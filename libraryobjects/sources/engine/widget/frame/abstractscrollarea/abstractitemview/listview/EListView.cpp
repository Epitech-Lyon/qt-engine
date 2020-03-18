/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EListView
*/

#include "EListView.hpp"
#include "EAbstractItemView.hpp"

#include "LibraryFunction.hpp"

template<> void libraryObjects::EListView::init(AObject *object)
{
	EAbstractItemView::init(object);
}

template<> QString libraryObjects::EListView::classIncludePath()
{
	return "#include <QtWidgets/QListView>";
}

template<> QIcon libraryObjects::EListView::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EListView::libraryFunction()
{
	auto libraryFunction = EAbstractItemView::libraryFunction();

	return libraryFunction;
}
