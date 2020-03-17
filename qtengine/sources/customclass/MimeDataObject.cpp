/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** MimeDataObject
*/

#include "MimeDataObject.hpp"

#include "ObjectClass.hpp"

qtengine::MimeDataObject::MimeDataObject(libraryObjects::ObjectClass *objectClass, libraryObjects::LibraryObject *libraryObject, libraryObjects::AObject *reference)
	: _objectClass(objectClass)
	, _libraryObject(libraryObject)
	, _reference(reference)
{
}

qtengine::MimeDataObject::~MimeDataObject()
{
	delete _objectClass;
}
