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
#include <functional>

namespace libraryObjects {
	class AObject;
	class LibraryFunction;

	class LibraryObject {
	public:
		typedef std::function<AObject *()> Constructor;

	public:
		LibraryObject(Constructor constructor, const QString &classHierarchy, const QIcon &icon, LibraryFunction *libraryFunction);
		~LibraryObject() = default;

		AObject *constructor() const { return _constructor(); }
		QString classHierarchy() const { return _classHierarchy; }
		QString className() const { return _className; }
		QIcon icon() const { return _icon; }
		LibraryFunction *libraryFunction() const { return _libraryFunction; }

	private:
		Constructor _constructor;
		QString _classHierarchy;
		QString _className;
		QIcon _icon;
		LibraryFunction *_libraryFunction;
	};
}
