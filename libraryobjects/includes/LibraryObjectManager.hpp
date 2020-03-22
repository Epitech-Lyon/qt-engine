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
#include "ClassTypeManager.hpp"

namespace libraryObjects {
	class LibraryObjectManager {
	public:
		~LibraryObjectManager();
		static LibraryObjectManager *instance();

		QList<LibraryObject *> libraryObjects() const;
		LibraryObject *libraryObjectOf(const QString &classHierarchy) const;
		LibraryObject *libraryObjectOfClassName(const QString &className) const;
		LibraryObject *libraryObjectOfType(const QString &type) const;

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
			auto libraryObject = new LibraryObject(constructor, Object::serializeData, Object::deserializeData, Object::classHierarchy(), Object::classIncludePath(), Object::icon(), Object::libraryFunction());

			_libraryObjects << libraryObject;
			types::ClassTypeManager::instance()->registerType(libraryObject->className() + "*");
		}

		QList<LibraryObject *> _libraryObjects;
		QMap<QString, LibraryObject *> _customObjects;
	};
}
