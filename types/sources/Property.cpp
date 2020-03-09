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
	: _userType(true)
	, _access(QMetaMethod::Access::Private)
	, _type(QMetaType::Type::QString)
{
}

types::Property::Property(const QMetaProperty &metaProperty)
	: _userType(false)
	, _access(QMetaMethod::Access::Private)
	, _type(static_cast<QMetaType::Type>(metaProperty.type()))
	, _name(metaProperty.name())
	, _setterName(metaProperty.isWritable() ? "setProperty" : "")
	, _getterName(metaProperty.isReadable() ? "property" : "")
{
}

types::Property::Property(const Property &property)
	: IType()
	, _userType(true)
	, _access(property.access())
	, _type(property.type())
	, _name(property.name())
	, _setterName(property.setterName())
	, _getterName(property.getterName())
{
}

types::Property &types::Property::operator=(const Property &property)
{
	if (property.isValid()) {
		_userType = property.isUserType();
		setAccess(property.access());
		setType(property.type());
		setName(property.name());
		setSetterName(property.setterName());
		setGetterName(property.getterName());
	}
	return *this;
}

QJsonObject types::Property::serialize() const
{
	if (!_userType) { return QJsonObject(); }

	QJsonObject json;
	json["access"] = static_cast<int>(_access);
	json["type"] = static_cast<int>(_type);
	json["name"] = _name;
	json["setterName"] = _setterName;
	json["getterName"] = _getterName;
	return json;
}

void types::Property::deserialize(const QJsonObject &json)
{
	_userType = true;
	_access = static_cast<QMetaMethod::Access>(json["access"].toInt());
	_type = static_cast<QMetaType::Type>(json["type"].toInt());
	_name = json["name"].toString();
	_setterName = json["setterName"].toString();
	_getterName = json["getterName"].toString();
}

QString types::Property::setterSignature() const
{
	if (!isValid() || _setterName.isEmpty()) { return ""; }

	return _userType
		? "void " + _setterName + "(" + QMetaType::typeName(_type) + ")"
		: "void " + _setterName + "(\"" + _name + "\", " + QMetaType::typeName(_type) + ")";
}

QString types::Property::getterSignature() const
{
	if (!isValid() || _getterName.isEmpty()) { return ""; }

	return _userType
		? QString(QMetaType::typeName(_type)) + " " + _getterName + "() const"
		: QString(QMetaType::typeName(_type)) + " " + _getterName + "(\"" + _name + "\")";
}

QDebug operator<<(QDebug debug, const types::Property &property)
{
	debug.nospace().noquote() << "Property(";
	if (property.isValid()) {
		debug << types::accessToString(property.access()).toLower();
		debug << " " << QMetaType::typeName(property.type()) << " " << property.name();

		QString accesseur = property.setterSignature();
		accesseur = accesseur.isEmpty() ? accesseur : accesseur + ", ";
		accesseur += property.getterSignature();
		if (!accesseur.isEmpty())
			debug << " [" << accesseur << "]";
	} else
		debug << "INVALID";
	debug << ")";
	return debug;
}
