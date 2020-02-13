/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewConverter
*/

#include "ViewConverter.hpp"
#include "AObject.hpp"

#include <QtCore/QJsonArray>

QJsonObject libraryObjects::ViewConverter::serialize(AObject *object)
{
	QJsonArray childJsonArray;
	for (auto child : object->children())
		childJsonArray << serialize(child);

	QJsonObject jsonData;
	jsonData["Children"] = childJsonArray;
	jsonData["Properties"] = object->serialize();

	QJsonObject json;
	json[object->className()] = jsonData;
	return json;
}

libraryObjects::AObject *libraryObjects::ViewConverter::deserialize(const QJsonObject &)
{
	return nullptr;
}
