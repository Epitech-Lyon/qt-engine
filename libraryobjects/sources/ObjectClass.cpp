/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectClass
*/

#include "ObjectClass.hpp"

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

types::ClassType *libraryObjects::ObjectClass::addClassType(types::ClassType *classType)
{
	if (!classType->isValid()) { return nullptr; }

	for (auto tmpClassType : _classTypes[classType->type()])
		if (tmpClassType->signature() == classType->signature())
			return tmpClassType;

	_classTypes[classType->type()].append(classType);
	return classType;
}
