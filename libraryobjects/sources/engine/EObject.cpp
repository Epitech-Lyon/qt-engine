/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EObject
*/

#include "EObject.hpp"

#include "LibraryFunction.hpp"

template<> void libraryObjects::EObject::init(AObject *object)
{
	connect(object->object(), &QObject::objectNameChanged, [object](const QString &objectName) { emit object->propertyUpdated("objectName", objectName); });
}

template<> libraryObjects::LibraryFunction *libraryObjects::EObject::libraryFunction()
{
	return new LibraryFunction();
}
