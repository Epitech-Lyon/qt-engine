/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectConverter
*/

#include "ObjectConverter.hpp"

#include "QVariantConverter.hpp"
#include <QtCore/QMetaProperty>
#include <QtCore/QDebug>

qtengine::ObjectConverter::ObjectConverter(QObject *object)
	: _object(object)
	, _className(_object->metaObject()->className())
{
	initProperties(_object->metaObject());
}

void qtengine::ObjectConverter::initProperties(const QMetaObject *metaObject)
{
	auto metaObjectSuperClass = metaObject->superClass();
	if (metaObjectSuperClass)
		initProperties(metaObjectSuperClass);

	for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); i += 1) {
		auto metaProperty = metaObject->property(i);

		if (metaProperty.isDesignable() && metaProperty.isWritable() && metaProperty.isReadable() && metaProperty.isStored())
			_properties[metaProperty.name()] = metaProperty.typeName();
	}
}

QJsonObject qtengine::ObjectConverter::serialize() const
{
	QJsonObject json;

	for (auto property : propertiesName()) {
		auto value = QVariantConverter::serialize(_object->property(property));

		if (value.isNull())
			qCritical() << "Serialize" << property << "of type" << _properties[property] << "is not implemented yet !";
		else
			json[property] = value;
	}
	verifyProperties(json);
	return json;
}

void qtengine::ObjectConverter::deserialize(const QJsonObject &)
{
}

void qtengine::ObjectConverter::verifyProperties(QJsonObject &jsonProperties) const
{
	if (jsonProperties["objectName"].toString().isEmpty())
		jsonProperties["objectName"] = _className;
}
