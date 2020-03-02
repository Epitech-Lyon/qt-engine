/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObject
*/

#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtGui/QIcon>
#include <QtCore/QJsonObject>
#include <functional>

namespace libraryObjects {
	class AObject;
	class LibraryFunction;

	class LibraryObject {
	public:
		typedef std::function<AObject *()> Constructor;
		typedef std::function<QJsonObject (AObject *)> SerializeData;
		typedef std::function<void (const QJsonObject &, AObject *)> DeserializeData;

	public:
		LibraryObject(Constructor constructor, SerializeData funSerialize, DeserializeData funDeserialize, const QString &classHierarchy, const QIcon &icon, LibraryFunction *libraryFunction);
		~LibraryObject();

		AObject *constructor() const { return _constructor(); }
		QJsonObject serializeData(AObject *object) const { return _funSerialize(object); }
		void deserializeData(const QJsonObject &json, AObject *object) const { _funDeserialize(json, object); }
		QString classHierarchy() const { return _classHierarchy; }
		QString className() const { return _className; }
		QIcon icon() const { return _icon; }
		LibraryFunction *libraryFunction() const { return _libraryFunction; }

	private:
		Constructor _constructor;
		SerializeData _funSerialize;
		DeserializeData _funDeserialize;
		QString _classHierarchy;
		QString _className;
		QIcon _icon;
		LibraryFunction *_libraryFunction;
	};
}
