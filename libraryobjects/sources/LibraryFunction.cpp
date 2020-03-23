/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryFunction
*/

#include "LibraryFunction.hpp"
#include "LibraryObjectManager.hpp"

libraryObjects::LibraryFunction::FunctionDrag libraryObjects::LibraryFunction::functionDragFor(const QString &classHierarchy) const
{
	for (auto functionDragAcceptedFor : _functionsDrag.keys())
		if (LibraryObjectManager::isSubClassOf(classHierarchy, functionDragAcceptedFor))
			return _functionsDrag[functionDragAcceptedFor];
	return FunctionDrag();
}
