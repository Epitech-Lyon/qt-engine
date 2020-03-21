/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectManager
*/

#include "ObjectManager.hpp"
#include "AObject.hpp"

libraryObjects::ObjectManager *libraryObjects::ObjectManager::instance()
{
	static ObjectManager objectManager;

	return &objectManager;
}

void libraryObjects::ObjectManager::registerObject(AObject *object)
{
	auto objectName = object->objectName();

	if (objectName.isEmpty()) {
		objectName = object->className();
		objectName = objectName.startsWith("Q") ? objectName.remove(0, 1) : objectName;
		objectName = objectName.replace(0, 1, objectName.front().toLower());
	}
	if (_objects.contains(objectName)) {
		int i = 1;
		while (this->object(objectName + "_" + QString::number(i)))
			i += 1;
		objectName = objectName + "_" + QString::number(i);
	}
	object->setObjectName(objectName);
	connect(object, &AObject::propertyUpdated, this, &ObjectManager::onPropertyUpdated);
	_objects[object->objectName()] = object;
	_objectsId[object->id()] = object;
}

void libraryObjects::ObjectManager::unregisterObject(AObject *object)
{
	disconnect(object, &AObject::propertyUpdated, this, &ObjectManager::onPropertyUpdated);
	_objects.remove(_objects.key(object));
	_objectsId.remove(object->id());
}

void libraryObjects::ObjectManager::onPropertyUpdated(const QString &propertyName, const QVariant &propertyValue)
{
	if (propertyName == "objectName") {
		auto object = dynamic_cast<AObject*>(sender());
		auto oldObjectName = objectName(object);
		auto newObjectName = propertyValue.toString();

		if (oldObjectName != newObjectName) {
			if (this->object(newObjectName))
				object->setObjectName(oldObjectName);
			else {
				_objects.remove(oldObjectName);
				_objects[newObjectName] = object;
			}
		}
	}
}

QString libraryObjects::ObjectManager::objectClassName(const QUuid &objectId) const
{
	auto object = this->object(objectId);

	return object ? object->className() : "";
}

QString libraryObjects::ObjectManager::objectName(const QUuid &objectId) const
{
	auto object = this->object(objectId);

	return object ? object->objectName() : "";
}
