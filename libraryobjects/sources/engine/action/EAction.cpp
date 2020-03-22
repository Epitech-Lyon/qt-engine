/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EAction
*/

#include "EAction.hpp"
#include "EObject.hpp"

template<> void libraryObjects::EAction::init(AObject *object)
{
	EObject::init(object);
}

template<> QString libraryObjects::EAction::classIncludePath()
{
	return "#include <QtWidgets/QAction>";
}

template<> QIcon libraryObjects::EAction::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EAction::libraryFunction()
{
	return EObject::libraryFunction();
}
