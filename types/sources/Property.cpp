/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Property
*/

#include "moc_Property.cpp"
#include "Property.hpp"

#include "ClassTypeManager.hpp"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qtgroupboxpropertybrowser.h"

#include "Utils.hpp"
#include <QtCore/QDebug>

types::Property::Property()
	: ClassType(QMetaMethod::Access::Private, Type::PROPERTY)
	, _userType(true)
	, _type(ClassTypeManager::instance()->type(QMetaType::QString))
{
}

types::Property::Property(const QMetaProperty &metaProperty)
	: ClassType(QMetaMethod::Access::Private, Type::PROPERTY)
	, _userType(false)
	, _type(QMetaType::typeName(metaProperty.type()))
	, _name(metaProperty.name())
	, _setterName(metaProperty.isWritable() ? "setProperty" : "")
	, _getterName(metaProperty.isReadable() ? "property" : "")
{
}

QJsonObject types::Property::serialize() const
{
	QJsonObject json = ClassType::serialize();
	json["isUserType"] = _userType;
	json["typeName"] = _type;
	json["name"] = _name;
	json["setterName"] = _setterName;
	json["getterName"] = _getterName;
	return json;
}

void types::Property::deserialize(const QJsonObject &json)
{
	ClassType::deserialize(json);
	_userType = json["isUserType"].toBool();
	_type = json["typeName"].toString();
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
		auto property = propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), "Type");

		property->setAttribute("enumNames", ClassTypeManager::instance()->types());
		property->setValue(ClassTypeManager::instance()->types().indexOf(_type));
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this](const QVariant &value) {
			setType(ClassTypeManager::instance()->types()[value.toInt()]);
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
	{
		auto property = propertyManager->addProperty(QVariant::String, "Setter name");

		property->setValue(_setterName);
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this](const QVariant &value) {
			setSetterName(value.toString());
		};
	}
	{
		auto property = propertyManager->addProperty(QVariant::String, "Getter name");

		property->setValue(_getterName);
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this](const QVariant &value) {
			setGetterName(value.toString());
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
		? "void " + _setterName + "(" + _type + ")"
		: "void " + _setterName + "(const char *name, const QVariant &value)";
}

QString types::Property::getterSignature() const
{
	if (!isValid() || _getterName.isEmpty()) { return ""; }

	return _userType
		? _type + " " + _getterName + "() const"
		: _type + " " + _getterName + "(const char *name) const";
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
