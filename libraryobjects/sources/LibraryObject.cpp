/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObject
*/

#include "LibraryObject.hpp"
#include "AObject.hpp"

#include "LibraryFunction.hpp"

libraryObjects::LibraryObject::LibraryObject(Constructor constructor, SerializeData funSerialize, DeserializeData funDeserialize, const QString &classHierarchy, const QIcon &icon, LibraryFunction *libraryFunction)
	: _constructor(constructor)
	, _funSerialize(funSerialize)
	, _funDeserialize(funDeserialize)
	, _classHierarchy(classHierarchy)
	, _className(_classHierarchy.split("::").last())
	, _icon(icon)
	, _libraryFunction(libraryFunction)
{
}

libraryObjects::LibraryObject::~LibraryObject()
{
	delete _libraryFunction;
}
