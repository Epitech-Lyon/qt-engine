/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ETextEdit
*/

#include "ETextEdit.hpp"
#include "EObject.hpp"

template<> QIcon libraryObjects::ETextEdit::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ETextEdit::libraryFunction()
{
	return EObject::libraryFunction();
}
