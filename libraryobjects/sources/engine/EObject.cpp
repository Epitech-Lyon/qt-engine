/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EObject
*/

#include "EObject.hpp"
#include "LibraryFunction.hpp"

template<> libraryObjects::LibraryFunction *libraryObjects::EObject::libraryFunction()
{
	return new LibraryFunction();
}
