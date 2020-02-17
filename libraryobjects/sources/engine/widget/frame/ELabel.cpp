/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ELabel
*/

#include "ELabel.hpp"
#include "EObject.hpp"

template<> QIcon libraryObjects::ELabel::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ELabel::libraryFunction()
{
	return EObject::libraryFunction();
}
