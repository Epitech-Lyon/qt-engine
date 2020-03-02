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
	auto libraryObject = LibraryObjectManager::instance()->libraryObjectOf(object->classHierarchy());
	if (!libraryObject) { return QJsonObject(); }

	QJsonArray childJsonArray;
	for (auto child : object->children())
		childJsonArray << serialize(child);

	QJsonObject jsonData;
	jsonData["Children"] = childJsonArray;
	jsonData["Data"] = libraryObject->serializeData(object);
	jsonData["Properties"] = object->serializeProperties();

	QJsonObject json;
	json[object->className()] = jsonData;
	return json;
}

libraryObjects::AObject *libraryObjects::ViewConverter::deserialize(const QJsonObject &json, bool isRoot)
{
	if (json.keys().isEmpty()) { return nullptr; }

	auto className = json.keys().front();
	auto libraryObject = LibraryObjectManager::instance()->libraryObjectOfClassName(className);
	auto object = libraryObject ? libraryObject->constructor() : nullptr;
	if (!object) { return nullptr; }

	auto jsonData = json[className].toObject();
	int index = 0;

	for (auto childRef : jsonData["Children"].toArray()) {
		auto child = deserialize(childRef.toObject(), false);

		if (child) {
			auto function = libraryObject->libraryFunction()->functionDragFor(child->classHierarchy());

			if (function.isValid && function.functionAdd(object, index, child))
				index += 1;
			_objectData[child]();
			_objectProperties[child]();
		}
	}
	_objectData[object] = std::bind(&LibraryObject::deserializeData, libraryObject, jsonData["Data"].toObject(), object);
	_objectProperties[object] = std::bind(&AObject::deserializeProperties, object, jsonData["Properties"].toObject());
	if (isRoot) {
		_objectData[object]();
		_objectProperties[object]();
	}
	return object;
}
