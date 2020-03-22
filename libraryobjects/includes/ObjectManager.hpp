/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectManager
*/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QUuid>

namespace libraryObjects {
	class AObject;

	class ObjectManager : public QObject {
	public:
		~ObjectManager() = default;
		static ObjectManager *instance();

		void registerObject(AObject *object);
		void unregisterObject(AObject *object);

		void setObjectAsRoot(AObject *rootObject) { _rootObject = rootObject; }

		AObject *object(const QString &objectName) const { return _objects.contains(objectName) ? _objects[objectName] : nullptr; }
		QString objectName(AObject *object) const { return _objects.key(object); }

		AObject *object(const QUuid &objectId) const { return _objectsId.contains(objectId) ? _objectsId[objectId] : nullptr; }
		QString objectClassName(const QUuid &objectId) const;
		QString objectName(const QUuid &objectId, bool returnThis = false) const;

	private slots:
		void onPropertyUpdated(const QString &propertyName, const QVariant &propertyValue);

	private:
		ObjectManager() = default;
		QMap<QString, AObject*> _objects;
		QMap<QUuid, AObject *> _objectsId;
		AObject *_rootObject = nullptr;
	};
}
