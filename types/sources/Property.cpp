/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Property
*/

#include "moc_Property.cpp"
#include "Property.hpp"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qtgroupboxpropertybrowser.h"

#include "Utils.hpp"
#include <QtCore/QDebug>

types::Property::Property()
	: ClassType(QMetaMethod::Access::Private, Type::PROPERTY)
	, _userType(true)
	, _type(QMetaType::Type::QString)
{
}

types::Property::Property(const QMetaProperty &metaProperty)
	: ClassType(QMetaMethod::Access::Private, Type::PROPERTY)
	, _userType(false)
	, _type(static_cast<QMetaType::Type>(metaProperty.type()))
	, _name(metaProperty.name())
	, _setterName(metaProperty.isWritable() ? "setProperty" : "")
	, _getterName(metaProperty.isReadable() ? "property" : "")
{
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

QWidget *types::Property::initEditor()
{
	auto propertyEditor = new QtGroupBoxPropertyBrowser();
	auto propertyFactory = new QtVariantEditorFactory(propertyEditor);
	auto propertyManager = new QtVariantPropertyManager(propertyFactory);

	propertyEditor->setFactoryForManager(propertyManager, propertyFactory);

	auto propertySlot = new QMap<QtProperty *, std::function<void (const QVariant &)>>;
	{
		auto property = propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), "Access");
		QStringList accessString;

		for (auto access : {QMetaMethod::Private, QMetaMethod::Protected, QMetaMethod::Public})
			accessString << types::accessToString(access);
		property->setAttribute("enumNames", accessString);
		property->setValue(accessString.indexOf(types::accessToString(_access)));
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this](const QVariant &value) {
			setAccess(static_cast<QMetaMethod::Access>(value.toInt()));
		};
	}
	{
		QStringList typeString;
		QList<QMetaType::Type> types;
		for (int i = QMetaType::UnknownType + 1; i < QMetaType::User + 1; i += 1)
			if (QMetaType::isRegistered(i) && i != QMetaType::Void) {
				typeString.append(QMetaType::typeName(i));
				types.append(static_cast<QMetaType::Type>(i));
			}
		auto property = propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), "Type");

		property->setAttribute("enumNames", typeString);
		property->setValue(typeString.indexOf(QMetaType::typeName(_type)));
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this, types](const QVariant &value) {
			setType(types[value.toInt()]);
		};
	}
	{
		auto property = propertyManager->addProperty(QVariant::String, "Name");

		property->setValue(_name);
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this](const QVariant &value) {
			setName(value.toString());
		};
	}

	connect(propertyManager, &QtVariantPropertyManager::valueChanged, [propertySlot](QtProperty *property, const QVariant &value) {
		if (propertySlot->contains(property))
			(*propertySlot)[property](value);
	});
	connect(propertyManager, &QObject::destroyed, [propertySlot]() {
		delete propertySlot;
	});
	return propertyEditor;
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
		debug << " " << property.signature();

		QString accesseur = property.setterSignature();
		accesseur = accesseur.isEmpty() ? accesseur : accesseur + ", ";
		accesseur += property.getterSignature();
		if (!accesseur.isEmpty())
			debug << " [" << accesseur << "]";
	} else
		debug << "INVALID";
	debug << ")";
	return debug.maybeSpace().maybeQuote();
}

QDebug operator<<(QDebug debug, const types::Property *property)
{
	return debug << *property;
}
