/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewConverter
*/

#include "ViewConverter.hpp"
#include "ObjectConverter.hpp"

qtengine::ViewConverter::ViewConverter(QObject *object)
	: _object(object)
{
}

QJsonObject qtengine::ViewConverter::serialize(QObject *object) const
{
	QJsonArray childJsonArray;
	for (auto child : object->children()) {
		QJsonObject childJson;

		childJson[child->metaObject()->className()] = serialize(child);
		childJsonArray << childJson;
	}

	QJsonObject json;
	json["Children"] = childJsonArray;
	json["Properties"] = ObjectConverter(object).serialize();
	return json;
}

QJsonObject qtengine::ViewConverter::serialize() const
{
	QJsonObject json;
	
	json[_object->metaObject()->className()] = serialize(_object);
	return json;
}

void qtengine::ViewConverter::deserialize(const QJsonObject &)
{
}
