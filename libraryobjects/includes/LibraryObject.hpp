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

	class LibraryObject {
	public:
		typedef std::function<AObject *()> Constructor;
		typedef std::function<AObject* (AObject*, LibraryObject *, int)> FunctionAdd;
		typedef std::function<void (AObject*, AObject*)> FunctionRemove;
		typedef QPair<FunctionAdd, FunctionRemove> Functions;

	public:
		LibraryObject(const QString &classHierarchy, const QIcon &icon, Constructor constructor, const QMap<QString, Functions> &functions);
		~LibraryObject() = default;

		QString classHierarchy() const { return _classHierarchy; }
		QString className() const { return _className; }
		QIcon icon() const { return _icon; }

		AObject *constructor() const { return _constructor(); }

		AObject *functionAdd(AObject *parent, LibraryObject *child, int position) const;
		void functionRemove(AObject *parent, AObject *child) const;

	private:
		Functions function(const QString &childClassHierarchy) const;

		QString _classHierarchy;
		QString _className;
		QIcon _icon;
		Constructor _constructor;
		QMap<QString, Functions> _functions;
	};
}
