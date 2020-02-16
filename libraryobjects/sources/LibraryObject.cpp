/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObject
*/

#include "LibraryObject.hpp"
#include "AObject.hpp"

libraryObjects::LibraryObject::LibraryObject(Constructor constructor, const QString &classHierarchy, const QIcon &icon, LibraryFunction *libraryFunction)
	: _constructor(constructor)
	, _classHierarchy(classHierarchy)
	, _className(_classHierarchy.split("::").last())
	, _icon(icon)
	, _libraryFunction(libraryFunction)
{
}
