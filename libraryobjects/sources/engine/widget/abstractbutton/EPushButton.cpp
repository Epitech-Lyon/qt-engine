/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EPushButton
*/

#include "EPushButton.hpp"
#include "EAbstractButton.hpp"

template<> void libraryObjects::EPushButton::init(AObject *object)
{
	EAbstractButton::init(object);
}

template<> QString libraryObjects::EPushButton::classIncludePath()
{
	return "#include <QtWidgets/QPushButton>";
}

template<> QIcon libraryObjects::EPushButton::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EPushButton::libraryFunction()
{
	return EAbstractButton::libraryFunction();
}
