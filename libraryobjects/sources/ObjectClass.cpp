/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectClass
*/

#include "ObjectClass.hpp"

QJsonObject libraryObjects::ObjectClass::serialize() const
{
	QJsonObject json;
	json["constructors"] = serializeTypes(_constructors);
	json["methods"] = serializeTypes(_methods);
	json["signals"] = serializeTypes(_signals);
	json["slots"] = serializeTypes(_slots);
	json["properties"] = serializeTypes(_properties);
	return json;
}

void libraryObjects::ObjectClass::deserialize(const QJsonObject &json)
{
	_constructors = deserializeTypes<types::Constructor>(json["constructors"].toArray());
	_methods = deserializeTypes<types::Method>(json["methods"].toArray());
	_signals = deserializeTypes<types::Method>(json["signals"].toArray());
	_slots = deserializeTypes<types::Method>(json["slots"].toArray());
	_properties = deserializeTypes<types::Property>(json["properties"].toArray());
}
