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
	initProperties(object->metaObject());
}

libraryObjects::AObject::~AObject()
{
	for (auto child : children())
		delete child;
	if (_parent)
		_parent->removeChild(this);
	delete _object;
}

QJsonObject libraryObjects::AObject::serialize() const
{
	QJsonObject json;

	for (auto className : classHierarchy().split("::"))
		for (auto &property : properties(className)) {
			auto value = QVariantConverter::serialize(propertyValue(property.name));

			if (value.isNull())
				qCritical() << "Serialize" << property.name << "of type" << property.typeName << "is not implemented yet !";
			else
				json[property.name] = value;
		}
	return json;
}

void libraryObjects::AObject::deserialize(const QJsonObject &json)
{
	for (auto key : json.keys()) {
		auto value = QVariantConverter::deserialize(QVariant::nameToType(key.toStdString().c_str()), json[key]);

		if (!value.isNull())
			setPropertyValue(key, value);
	}
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

void libraryObjects::AObject::setPropertyValue(const QString &propertyName, const QVariant &propertyValue)
{
	_object->setProperty(propertyName.toStdString().c_str(), propertyValue);
}

void libraryObjects::AObject::insertChild(int index, AObject *child)
{
	if (!child || index < 0 || index > _children.size()) { return; }

	_children.removeAll(child);
	_children.insert(index, child);
	child->_parent = this;
}

void libraryObjects::AObject::addChild(AObject *child)
{
	insertChild(_children.size(), child);
}

void libraryObjects::AObject::removeChild(AObject *child)
{
	if (!child) { return; }

	_children.removeAll(child);
	child->_parent = nullptr;
	child->object()->setParent(nullptr);
}
