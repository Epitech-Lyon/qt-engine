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
		LibraryObject *libraryObjectOfClassName(const QString &className) const;

	private:
		LibraryObjectManager();
		template <typename Object> void registerObject()
		{
			auto constructor = []() { return new Object(); };

			_libraryObjects << new LibraryObject(constructor, Object::classHierarchy(), Object::icon(), Object::libraryFunction());
		}

		QList<LibraryObject *> _libraryObjects;
	};
}
