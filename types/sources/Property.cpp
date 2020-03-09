/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Property
*/

#include "moc_Property.cpp"
#include "Property.hpp"

#include "Utils.hpp"
#include <QtCore/QDebug>

types::Property::Property()
	: _access(QMetaMethod::Access::Private)
	, _type(QMetaType::Type::QString)
	, _name("test")
	, _haveSetter(true)
	, _haveGetter(true)
{
}

types::Property::Property(const QMetaProperty &metaProperty)
	: _access(QMetaMethod::Access::Private)
	, _type(static_cast<QMetaType::Type>(metaProperty.type()))
	, _name(metaProperty.name())
	, _haveSetter(metaProperty.isWritable())
	, _haveGetter(metaProperty.isReadable())
{
}

types::Property::Property(const Property &property)
	: IType()
	, _access(property.access())
	, _type(property.type())
	, _name(property.name())
	, _haveSetter(property.haveSetter())
	, _haveGetter(property.haveGetter())
{
}

types::Property &types::Property::operator=(const Property &property)
{
	if (property.isValid()) {
		setAccess(property.access());
		setType(property.type());
		setName(property.name());
		haveSetter(property.haveSetter());
		haveGetter(property.haveGetter());
	}
	return *this;
}

QJsonObject types::Property::serialize() const
{
	QJsonObject json;
	json["access"] = static_cast<int>(_access);
	json["type"] = static_cast<int>(_type);
	json["name"] = _name;
	json["haveSetter"] = _haveSetter;
	json["haveGetter"] = _haveGetter;
	return json;
}

void types::Property::deserialize(const QJsonObject &json)
{
	_access = static_cast<QMetaMethod::Access>(json["access"].toInt());
	_type = static_cast<QMetaType::Type>(json["type"].toInt());
	_name = json["name"].toString();
	_haveSetter = json["haveSetter"].toBool();
	_haveGetter = json["haveGetter"].toBool();
}

QDebug operator<<(QDebug debug, const types::Property &property)
{
	debug.nospace().noquote() << "Property(";
	if (property.isValid()) {
		debug << types::accessToString(property.access()).toLower();
		debug << " " << QMetaType::typeName(property.type()) << " " << property.name();
		if (property.haveSetter() && property.haveGetter())
			debug << " [Setter, Getter]";
		else if (property.haveSetter())
			debug << " [Setter]";
		else if (property.haveGetter())
			debug << " [Getter]";
	} else
		debug << "INVALID";
	debug << ")";
	return debug;
}
