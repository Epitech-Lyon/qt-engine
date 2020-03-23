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
		typedef std::function<QString (AObject *)> Code;

	public:
		LibraryObject(Constructor constructor, SerializeData funSerialize, DeserializeData funDeserialize, const QString &classHierarchy, const QString &classIncludePath, const QIcon &icon, LibraryFunction *libraryFunction, Code code);
		~LibraryObject();

		AObject *constructor() const { return _constructor(); }
		QJsonObject serializeData(AObject *object) const { return _funSerialize(object); }
		void deserializeData(const QJsonObject &json, AObject *object) const { _funDeserialize(json, object); }
		QString classHierarchy() const { return _classHierarchy; }
		QString className() const { return _className; }
		QString classIncludePath() const { return _classIncludePath; }
		QIcon icon() const { return _icon; }
		LibraryFunction *libraryFunction() const { return _libraryFunction; }
		QString code(AObject *object) const { return _code(object); }

	private:
		Constructor _constructor;
		SerializeData _funSerialize;
		DeserializeData _funDeserialize;
		QString _classHierarchy;
		QString _className;
		QString _classIncludePath;
		QIcon _icon;
		LibraryFunction *_libraryFunction;
		Code _code;
	};
}
