/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObjectManager
*/

#pragma once

#include <QtCore/QMetaObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>

#include "LibraryObject.hpp"
#include "ClassTypeManager.hpp"

namespace libraryObjects {
	class LibraryObjectManager {
	public:
		typedef std::function<const QMetaObject ()> FunctionMetaObject;

		~LibraryObjectManager();
		static LibraryObjectManager *instance();

		static bool isSubClassOf(const QString &classHierarchy, const QString &parentClassHierarchy);
		static bool isSubClassOf(const AObject *object, const AObject *objectParent);
		static bool isSubClassOf(const LibraryObject *libraryObject, const LibraryObject *libraryObjectParent);

		QList<LibraryObject *> libraryObjects() const;
		LibraryObject *libraryObjectOf(const QString &classHierarchy) const;
		LibraryObject *libraryObjectOfClassName(const QString &className) const;
		LibraryObject *libraryObjectOfType(const QString &type) const;

		void registerCustomObject(const QString &name, LibraryObject *libraryObject, FunctionMetaObject functionMetaObject);
		LibraryObject *customObject(const QString &name) const { return _customObjects[name]; }
		QList<LibraryObject *> customObjects() const { return _customObjects.values(); }
		void unregisterCustomObject(const QString &name);
		void unregisterAllCustomObjects();

		FunctionMetaObject metaObjectOf(const QString &classHierarchy) const;
		FunctionMetaObject metaObjectOfType(const QString &type) const;

	private:
		LibraryObjectManager();
		template <typename Object> void registerObject()
		{
			auto constructor = []() { auto object = new Object(); Object::init(object); return object; };
			auto libraryObject = new LibraryObject(constructor, Object::serializeData, Object::deserializeData, Object::classHierarchy(), Object::classIncludePath(), Object::icon(), Object::libraryFunction(), Object::code);

			_libraryObjects << libraryObject;
			_metaObjects[libraryObject] = Object::staticMetaObject;
			types::ClassTypeManager::instance()->registerType(libraryObject->className() + "*");
		}

		QMap<LibraryObject *, FunctionMetaObject> _metaObjects;
		QList<LibraryObject *> _libraryObjects;
		QMap<QString, LibraryObject *> _customObjects;
	};
}
