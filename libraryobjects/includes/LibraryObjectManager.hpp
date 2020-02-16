/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObjectManager
*/

#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

#include "LibraryObject.hpp"

namespace libraryObjects {
	class LibraryObjectManager {
	public:
		~LibraryObjectManager() = default;
		static LibraryObjectManager *instance();

		QList<LibraryObject *> libraryObjects() const { return _libraryObjects; }
		LibraryObject *libraryObjectOf(const QString &classHierarchy) const;

	private:
		LibraryObjectManager();
		template <typename Object> void registerObject()
		{
			_libraryObjects << new LibraryObject(Object::classHierarchy(), Object::icon(), Object::constructor(), Object::functionDrag());
		}

		QList<LibraryObject *> _libraryObjects;
	};
}
