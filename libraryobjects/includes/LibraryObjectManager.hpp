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
		~LibraryObjectManager();
		static LibraryObjectManager *instance();

		QList<LibraryObject *> libraryObjects() const { return _libraryObjects; }
		LibraryObject *libraryObjectOf(const QString &classHierarchy) const;
		LibraryObject *libraryObjectOfClassName(const QString &className) const;

	private:
		LibraryObjectManager();
		template <typename Object> void registerObject()
		{
			auto constructor = []() { auto object = new Object(); Object::init(object); return object; };

			_libraryObjects << new LibraryObject(constructor, Object::serializeData, Object::deserializeData, Object::classHierarchy(), Object::icon(), Object::libraryFunction());
		}

		QList<LibraryObject *> _libraryObjects;
	};
}
