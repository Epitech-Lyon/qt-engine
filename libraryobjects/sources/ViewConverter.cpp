/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewConverter
*/

#include "ViewConverter.hpp"
#include "AObject.hpp"

#include "LibraryObjectManager.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QMetaType>

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

libraryObjects::AObject *libraryObjects::ViewConverter::deserialize(const QJsonObject &json)
{
	auto libraryObjectmanager = LibraryObjectManager::instance();
	auto className = json.keys().front();
	auto libraryObject = libraryObjectmanager->libraryObjectOfClassName(className);
	if (!libraryObject) { return nullptr; }

	auto object = libraryObject->constructor();
	if (!object) { return nullptr; }

	auto jsonData = json[className].toObject();
	int index = 0;

	object->deserialize(jsonData["Properties"].toObject());
	for (auto childRef : jsonData["Children"].toArray()) {
		auto child = deserialize(childRef.toObject());

		if (child) {
			// TODO -> pas la bonne méthode
			object->addChild(child);
		}
		index += 1;
	}
	return object;
}
