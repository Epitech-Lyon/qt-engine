/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EVBoxLayout
*/

#include "EVBoxLayout.hpp"
#include "EBoxLayout.hpp"

template<> void libraryObjects::EVBoxLayout::init(AObject *object)
{
	EBoxLayout::init(object);
}

template<> QString libraryObjects::EVBoxLayout::classIncludePath()
{
	return "#include <QtWidgets/QVBoxLayout>";
}

template<> QIcon libraryObjects::EVBoxLayout::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EVBoxLayout::libraryFunction()
{
	return EBoxLayout::libraryFunction();
}
