/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObject
*/

#include "LibraryObject.hpp"
#include "AObject.hpp"

#include "LibraryFunction.hpp"

libraryObjects::LibraryObject::LibraryObject(Constructor constructor, SerializeData funSerialize, DeserializeData funDeserialize, const QString &classHierarchy, const QString &classIncludePath, const QIcon &icon, LibraryFunction *libraryFunction, Code code)
	: _constructor(constructor)
	, _funSerialize(funSerialize)
	, _funDeserialize(funDeserialize)
	, _classHierarchy(classHierarchy)
	, _className(_classHierarchy.split("::").last())
	, _classIncludePath(classIncludePath)
	, _icon(icon)
	, _libraryFunction(libraryFunction)
	, _code(code)
{
}

libraryObjects::LibraryObject::~LibraryObject()
{
	delete _libraryFunction;
}
