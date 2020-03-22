/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EProgressBar
*/

#include "EProgressBar.hpp"
#include "EObject.hpp"

template<> void libraryObjects::EProgressBar::init(AObject *object)
{
	EObject::init(object);
}

template<> QString libraryObjects::EProgressBar::classIncludePath()
{
	return "#include <QtWidgets/QProgressBar>";
}

template<> QIcon libraryObjects::EProgressBar::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EProgressBar::libraryFunction()
{
	return EObject::libraryFunction();
}
