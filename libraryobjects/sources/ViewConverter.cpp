/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewConverter
*/

#include "ViewConverter.hpp"
#include "AObject.hpp"

#include "LibraryObjectManager.hpp"
#include "LibraryFunction.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QMetaType>

QJsonObject libraryObjects::ViewConverter::serialize(AObject *object)
{
	QJsonArray childJsonArray;
	for (auto child : object->children())
		childJsonArray << serialize(child);

	QJsonObject jsonData;
	jsonData["Children"] = childJsonArray;
	jsonData["Properties"] = object->serializeProperties();
	jsonData["Data"] = object->serializeData();

	QJsonObject json;
	json[object->className()] = jsonData;
	return json;
}

libraryObjects::AObject *libraryObjects::ViewConverter::deserialize(const QJsonObject &json)
{
	if (json.keys().size() == 0) { return nullptr; }

	auto libraryObjectmanager = LibraryObjectManager::instance();
	auto className = json.keys().front();
	auto libraryObject = libraryObjectmanager->libraryObjectOfClassName(className);
	if (!libraryObject) { return nullptr; }

	auto object = libraryObject->constructor();
	if (!object) { return nullptr; }

	auto jsonData = json[className].toObject();
	int index = 0;

	for (auto childRef : jsonData["Children"].toArray()) {
		auto child = deserialize(childRef.toObject());

		if (child) {
			auto function = libraryObject->libraryFunction()->functionDragFor(child->classHierarchy());

			if (function.isValid && function.functionAdd(object, index, child))
				index += 1;
		}
	}
	object->deserializeData(jsonData["Data"].toObject());
	object->deserializeProperties(jsonData["Properties"].toObject());
	return object;
}
