/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObject
*/

#include "LibraryObject.hpp"
#include "AObject.hpp"

libraryObjects::LibraryObject::LibraryObject(const QString &classHierarchy, const QIcon &icon, Constructor constructor, const QPair<QString, FunctionDrag> &functionDrag)
	: _classHierarchy(classHierarchy)
	, _className(_classHierarchy.split("::").last())
	, _icon(icon)
	, _constructor(constructor)
	, _functionDrag(functionDrag)
{
}

bool libraryObjects::LibraryObject::canCallFunctionDrag(LibraryObject *child) const
{
	return !_functionDrag.first.isEmpty() && _functionDrag.second && child->classHierarchy().startsWith(_functionDrag.first);
}

libraryObjects::AObject *libraryObjects::LibraryObject::functionDrag(AObject *parent, int position, LibraryObject *child) const
{
	return canCallFunctionDrag(child) ? _functionDrag.second(parent, position, child) : nullptr;
}
