/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ELabel
*/

#include "ELabel.hpp"

template<> QIcon libraryObjects::ELabel::icon()
{
	return QIcon();
}

template<> QPair<QString, libraryObjects::LibraryObject::FunctionDrag> libraryObjects::ELabel::functionDrag()
{
	return QPair<QString, libraryObjects::LibraryObject::FunctionDrag>();
}
