/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectClass
*/

#include "ObjectClass.hpp"

#include "Constructor.hpp"
#include "Method.hpp"
#include "Signal.hpp"
#include "Slot.hpp"
#include "Property.hpp"

libraryObjects::ObjectClass::ObjectClass(const QMetaObject *metaObject)
{
	for (int idx = 0; idx < metaObject->constructorCount(); idx += 1)
		addClassType(new types::Constructor(metaObject->constructor(idx)));
	for (int idx = 0; idx < metaObject->methodCount(); idx += 1) {
		auto metaMethod = metaObject->method(idx);

		if (metaMethod.methodType() == QMetaMethod::Method)
			addClassType(new types::Method(metaMethod));
		else if (metaMethod.methodType() == QMetaMethod::Signal)
			addClassType(new types::Signal(metaMethod));
		else if (metaMethod.methodType() == QMetaMethod::Slot)
			addClassType(new types::Slot(metaMethod));
	}
	for (int idx = 0; idx < metaObject->propertyCount(); idx += 1)
		addClassType(new types::Property(metaObject->property(idx)));
}

libraryObjects::ObjectClass::~ObjectClass()
{
	for (auto classTypeList : _classTypes)
		for (auto classType : classTypeList)
			delete classType;
}

QJsonObject libraryObjects::ObjectClass::serialize() const
{
	auto serializeList = [](const QList<types::ClassType *> &classTypes) {
		QJsonArray jsonArray;

		for (auto classType : classTypes)
			jsonArray.append(classType->serialize());
		return jsonArray;
	};

	QJsonObject json;
	json["constructors"] = serializeList(_classTypes[types::ClassType::CONSTRUCTOR]);
	json["methods"] = serializeList(_classTypes[types::ClassType::METHOD]);
	json["signals"] = serializeList(_classTypes[types::ClassType::SIGNAL]);
	json["slots"] = serializeList(_classTypes[types::ClassType::SLOT]);
	json["properties"] = serializeList(_classTypes[types::ClassType::PROPERTY]);
	return json;
}

void libraryObjects::ObjectClass::deserialize(const QJsonObject &json)
{
	auto deserializeList = [](types::ClassType::Type type, const QJsonArray &jsonArray) {
			QList<types::ClassType*> types;

			for (auto jsonRef : jsonArray) {
				auto classType = types::ClassType::construct(type);

				classType->deserialize(jsonRef.toObject());
				if (classType->isValid())
					types.append(classType);
				else
					delete classType;
			}
			return types;
	};

	_classTypes[types::ClassType::CONSTRUCTOR] = deserializeList(types::ClassType::CONSTRUCTOR, json["constructors"].toArray());
	_classTypes[types::ClassType::METHOD] = deserializeList(types::ClassType::METHOD, json["methods"].toArray());
	_classTypes[types::ClassType::SIGNAL] = deserializeList(types::ClassType::SIGNAL, json["signals"].toArray());
	_classTypes[types::ClassType::SLOT] = deserializeList(types::ClassType::SLOT, json["slots"].toArray());
	_classTypes[types::ClassType::PROPERTY] = deserializeList(types::ClassType::PROPERTY, json["properties"].toArray());
}

types::ClassType *libraryObjects::ObjectClass::addClassType(types::ClassType *classTypeToAdd)
{
	if (!classTypeToAdd->isValid()) { return nullptr; }

	for (auto classTypeList : _classTypes)
		for (auto classType : classTypeList)
			if (classType->signature() == classTypeToAdd->signature())
				return classType;

	_classTypes[classTypeToAdd->type()].append(classTypeToAdd);
	return classTypeToAdd;
}
