/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EObject
*/

#include "EObject.hpp"

#include "LibraryFunction.hpp"

template<> void libraryObjects::EObject::init(AObject *)
{
}

template<> QString libraryObjects::EObject::classIncludePath()
{
	return "#include <QtCore/QObject>";
}

template<> QIcon libraryObjects::EObject::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EObject::libraryFunction()
{
	return new LibraryFunction();
}
