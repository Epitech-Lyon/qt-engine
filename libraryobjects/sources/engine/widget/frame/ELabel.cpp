/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ELabel
*/

#include "ELabel.hpp"
#include "EObject.hpp"

template<> void libraryObjects::ELabel::init(AObject *object)
{
	EObject::init(object);
}

template<> QString libraryObjects::ELabel::classIncludePath()
{
	return "#include <QtWidgets/QLabel>";
}

template<> QIcon libraryObjects::ELabel::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ELabel::libraryFunction()
{
	return EObject::libraryFunction();
}
