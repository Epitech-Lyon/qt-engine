/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewManagement
*/

#include "ViewManagement.hpp"
#include "ObjectManagement.hpp"

qtengine::ViewManagement::ViewManagement(QObject *object)
	: _object(object)
{
}

QJsonObject qtengine::ViewManagement::serialize(QObject *object) const
{
	QJsonArray childJsonArray;
	for (auto child : object->children()) {
		QJsonObject childJson;

		childJson[child->metaObject()->className()] = serialize(child);
		childJsonArray << childJson;
	}

	QJsonObject json;
	json["Children"] = childJsonArray;
	json["Properties"] = ObjectManagement(object).serialize();
	return json;
}

QJsonObject qtengine::ViewManagement::serialize() const
{
	QJsonObject json;
	
	json[_object->metaObject()->className()] = serialize(_object);
	return json;
}

void qtengine::ViewManagement::deserialize(const QJsonObject &)
{
}
