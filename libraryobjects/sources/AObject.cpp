/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** AObject
*/

#include "moc_AObject.cpp"
#include "AObject.hpp"

#include "ObjectManager.hpp"

#include "QVariantConverter.hpp"
#include <QtCore/QDebug>
#include <QtCore/QUuid>

libraryObjects::AObject::AObject(QObject *object, const QString &classHierarchy)
	: _id(QUuid::createUuid())
	, _object(object)
	, _classHierarchy(classHierarchy)
	, _className(classHierarchy.split("::").last())
	, _parent(nullptr)
{
	initProperties(object->metaObject());
	_isRegistered = ObjectManager::instance()->registerObject(this);
}

libraryObjects::AObject::~AObject()
{
	for (auto child : children())
		delete child;
	if (_parent)
		_parent->_children.removeAll(this);
	delete _object;
	if (_isRegistered)
		ObjectManager::instance()->unregisterObject(this);
}

void libraryObjects::AObject::removeObject()
{
	_object = nullptr;
	for (auto child : _children)
		child->removeObject();
}

QJsonObject libraryObjects::AObject::serializeProperties() const
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
	json["id"] = _id.toString();
	return json;
}

void libraryObjects::AObject::deserializeProperties(const QJsonObject &json)
{
	for (auto key : json.keys()) {
		if (key == "id") { continue; }

		auto value = QVariantConverter::deserialize(json[key]);

		if (!value.isNull())
			setPropertyValue(key, value);
	}
	if (_isRegistered)
		ObjectManager::instance()->unregisterObject(this);
	_id = QUuid(json["id"].toString());
	_isRegistered = ObjectManager::instance()->registerObject(this);
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
	if (this->propertyValue(propertyName) != propertyValue) {
		_object->setProperty(propertyName.toStdString().c_str(), propertyValue);
		emit propertyUpdated(propertyName, propertyValue);
	}
}

bool libraryObjects::AObject::isChildOf(AObject *object) const
{
	for (auto parent = _parent; parent; parent = parent->_parent)
		if (parent == object)
			return true;
	return false;
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
}
