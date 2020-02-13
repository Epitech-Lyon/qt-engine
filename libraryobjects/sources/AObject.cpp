/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** AObject
*/

#include "AObject.hpp"

#include "QVariantConverter.hpp"
#include <QtCore/QDebug>

libraryObjects::AObject::AObject(QObject *object, const QString &classHierarchy)
	: _object(object)
	, _classHierarchy(classHierarchy)
	, _className(classHierarchy.split("::").last())
{
}

void libraryObjects::AObject::initProperties(const QMetaObject *metaObject)
{
	auto metaObjectSuperClass = metaObject->superClass();
	if (metaObjectSuperClass)
		initProperties(metaObjectSuperClass);

	for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); i += 1) {
		auto metaProperty = metaObject->property(i);

		if (metaProperty.isDesignable() && metaProperty.isWritable() && metaProperty.isReadable() && metaProperty.isStored())
			_properties[metaObject->className()].push_back(metaProperty);
	}
}

QJsonObject libraryObjects::AObject::serialize() const
{
	QJsonObject json;

	for (auto className : classHierarchy())
		for (auto &property : properties(className)) {
			auto value = QVariantConverter::serialize(_object->property(property.name.toStdString().c_str()));

			if (value.isNull())
				qCritical() << "Serialize" << property.name << "of type" << property.typeName << "is not implemented yet !";
			else
				json[property.name] = value;
		}
	return json;
}

void libraryObjects::AObject::deserialize(const QJsonObject &)
{
}

void libraryObjects::AObject::addChild(AObject *child)
{
	if (!child) { return; }

	_children.removeAll(child);
	_children.push_back(child);
	child->_parent = this;
}

void libraryObjects::AObject::removeChild(AObject *child)
{
	if (!child) { return; }

	_children.removeAll(child);
	child->_parent = nullptr;
}
