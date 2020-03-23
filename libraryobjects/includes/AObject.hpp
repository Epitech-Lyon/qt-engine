/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** AObject
*/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QJsonObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QUuid>

namespace libraryObjects {
	class AObject : public QObject {
		Q_OBJECT

	// General management
	public:
		virtual ~AObject();

		QUuid id() const { return _id; }
		QObject *object() const { return _object; }
		void removeObject();
		QString objectName() const { return _object->objectName(); }
		QString classHierarchy() const { return _classHierarchy; }
		QString className() const { return _className; }

		void setObjectName(const QString &objectName) { setPropertyValue("objectName", objectName); }

		QJsonObject serializeProperties() const;
		void deserializeProperties(const QJsonObject &);

	protected:
		AObject(QObject *object, const QString &classHierarchy);

	private:
		QUuid _id;
		bool _isRegistered;
		QObject *_object;
		QString _classHierarchy;
		QString _className;

	// Parenty management
	public:
		AObject *parent() const { return _parent; }

		bool isChildOf(AObject *object) const;
		QList<AObject *> children() const { return _children; }
		void insertChild(int index, AObject *child);
		void addChild(AObject *child);
		void removeChild(AObject *child);

	private:
		AObject *_parent;
		QList<AObject *> _children;

	// Properties management
	public:
		typedef struct Property {
			Property(const QMetaProperty &metaProperty) : isFlag(metaProperty.isFlagType()), isEnum(metaProperty.isEnumType()), name(metaProperty.name()), typeName(metaProperty.typeName()) { initKeys(metaProperty.enumerator()); }
			Property(const Property &other) : isFlag(other.isFlag), isEnum(other.isEnum), keys(other.keys), name(other.name), typeName(other.typeName) {}
			Property &operator=(const Property &other) { isFlag = other.isFlag; isEnum = other.isEnum; keys = other.keys; name = other.name; typeName = other.typeName; return *this; }

			bool isFlag;
			bool isEnum;
			QStringList keys;
			QString name;
			QString typeName;

		private:
			void initKeys(const QMetaEnum &metaEnum)
			{
				for (int i = 0; i < metaEnum.keyCount(); i += 1)
					keys << metaEnum.key(i);
			}
		} Property;

		QList<Property> properties(const QString &className) const { return _properties[className]; }

		QVariant propertyValue(const QString &propertyName) const { return _object->property(propertyName.toStdString().c_str()); }
		void setPropertyValue(const QString &propertyName, const QVariant &propertyValue);

		QVariant dynamicProperty(const QString &propertyName, const QVariant &defautlValue = QVariant()) const;
		void addDynamicProperty(const QString &propertyName, const QVariant &value, const QString &code = "");
		void removeDynamicProperty(const QString &propertyName);

	signals:
		void propertyUpdated(const QString &propertyName, const QVariant &propertyValue) const;

	private:
		void initProperties(const QMetaObject *);
		QMap<QString, QList<Property>> _properties;
		QMap<QString, QPair<QVariant, QString>> _dynamicProperties;
	};
}
