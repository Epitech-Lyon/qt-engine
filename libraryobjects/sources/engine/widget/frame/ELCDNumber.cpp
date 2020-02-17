/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ELCDNumber
*/

#include "ELCDNumber.hpp"
#include "EObject.hpp"

template<> QIcon libraryObjects::ELCDNumber::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ELCDNumber::libraryFunction()
{
	return EObject::libraryFunction();
}
