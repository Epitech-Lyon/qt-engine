/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryFunction
*/

#include "LibraryFunction.hpp"

libraryObjects::LibraryFunction::FunctionDrag libraryObjects::LibraryFunction::functionDragFor(const QString &classHierarchy) const
{
	for (auto functionDragAcceptedFor : _functionsDrag.keys())
		if (classHierarchy.startsWith(functionDragAcceptedFor))
			return _functionsDrag[functionDragAcceptedFor];
	return FunctionDrag();
}
