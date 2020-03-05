/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectManager
*/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>

namespace libraryObjects {
	class AObject;

	class ObjectManager : public QObject {
	public:
		~ObjectManager() = default;
		static ObjectManager *instance();

		void registerObject(AObject *object);
		void unregisterObject(AObject *object);

		AObject *object(const QString &objectName) const { return _objects.contains(objectName) ? _objects[objectName] : nullptr; }
		QString objectName(AObject *object) const { return _objects.key(object); }

	private slots:
		void onPropertyUpdated(const QString &propertyName, const QVariant &propertyValue);

	private:
		ObjectManager() = default;
		QMap<QString, AObject*> _objects;
	};
}
