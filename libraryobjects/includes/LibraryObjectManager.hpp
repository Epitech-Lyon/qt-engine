/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObjectManager
*/

#pragma once

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>

#include "LibraryObject.hpp"

namespace libraryObjects {
	class LibraryObjectManager {
	public:
		~LibraryObjectManager();
		static LibraryObjectManager *instance();

		QList<LibraryObject *> libraryObjects() const;
		LibraryObject *libraryObjectOf(const QString &classHierarchy) const;
		LibraryObject *libraryObjectOfClassName(const QString &className) const;

		void registerCustomObject(const QString &name, LibraryObject *libraryObject);
		LibraryObject *customObject(const QString &name) const { return _customObjects[name]; }
		QList<LibraryObject *> customObjects() const { return _customObjects.values(); }
		void unregisterCustomObject(const QString &name);
		void unregisterAllCustomObjects();

	private:
		LibraryObjectManager();
		template <typename Object> void registerObject()
		{
			auto constructor = []() { auto object = new Object(); Object::init(object); return object; };

			_libraryObjects << new LibraryObject(constructor, Object::serializeData, Object::deserializeData, Object::classHierarchy(), Object::classIncludePath(), Object::icon(), Object::libraryFunction());
		}

		QList<LibraryObject *> _libraryObjects;
		QMap<QString, LibraryObject *> _customObjects;
	};
}
