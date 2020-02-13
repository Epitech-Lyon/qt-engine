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

namespace libraryObjects {
	class AObject {
	// Some structure
	public:
		typedef struct Variable {
			Variable(const QMetaProperty &metaProperty) : name(metaProperty.name()), typeName(metaProperty.typeName()) {}
			Variable(const Variable &other) : name(other.name), typeName(other.typeName) {}
			Variable &operator=(const Variable &other) { name = other.name; typeName = other.typeName; return *this; }

			QString name;
			QString typeName;
		} Variable;

	// General management
	public:
		AObject(QObject *object, const QString &classHierarchy);
		~AObject() = default;

		QObject *object() const { return _object; }
		QString classHierarchy() const { return _classHierarchy; }
		QString className() const { return _className; }

		QJsonObject serialize() const;
		void deserialize(const QJsonObject &);

	protected:
		QObject *_object;
		QString _classHierarchy;
		QString _className;

	// Parenty management
	public:
		QList<AObject *> children() const { return _children; }
		void addChild(AObject *child);
		void removeChild(AObject *child);

	private:
		AObject *_parent;
		QList<AObject *> _children;

	// Properties management
	public:
		QList<Variable> properties(const QString &className) const { return _properties[className]; }

	private:
		void initProperties(const QMetaObject *);
		QMap<QString, QList<Variable>> _properties;
	};
}
