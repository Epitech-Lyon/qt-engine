/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EPlainTextEdit
*/

#include "EPlainTextEdit.hpp"
#include "EObject.hpp"

template<> QIcon libraryObjects::EPlainTextEdit::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EPlainTextEdit::libraryFunction()
{
	return EObject::libraryFunction();
}
