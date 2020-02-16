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

		typedef std::function<AObject* (AObject*, int, LibraryObject *)> FunctionDrag;

//		typedef std::function<void (AObject *)> FunctionMenuAdd;
//		typedef std::function<void (AObject*)> FunctionMenuRemove;
//		typedef QPair<FunctionAdd, FunctionRemove> FunctionsMenu;

	public:
		LibraryObject(const QString &classHierarchy, const QIcon &icon, Constructor constructor, const QPair<QString, FunctionDrag> &functionDrag);
		~LibraryObject() = default;

		QString classHierarchy() const { return _classHierarchy; }
		QString className() const { return _className; }
		QIcon icon() const { return _icon; }

		AObject *constructor() const { return _constructor(); }

		bool canCallFunctionDrag(LibraryObject *child) const;
		AObject *functionDrag(AObject *parent, int position, LibraryObject *child) const;

	private:
		QString _classHierarchy;
		QString _className;
		QIcon _icon;
		Constructor _constructor;
		QPair<QString, FunctionDrag> _functionDrag;
	};
}
