/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EToolButton
*/

#include "EToolButton.hpp"
#include "EAbstractButton.hpp"

template<> void libraryObjects::EToolButton::init(AObject *object)
{
	EAbstractButton::init(object);
}

template<> QString libraryObjects::EToolButton::classIncludePath()
{
	return "#include <QtWidgets/QToolButton>";
}

template<> QIcon libraryObjects::EToolButton::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EToolButton::libraryFunction()
{
	return EAbstractButton::libraryFunction();
}
