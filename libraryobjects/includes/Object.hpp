/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Object
*/

#pragma once

#include "AObject.hpp"

#include "LibraryObject.hpp"

namespace libraryObjects {
	template <typename Obj> class Object : public AObject {
	public:
		Object()
			: AObject(new Obj, classHierarchy())
		{
		}

		~Object() = default;

		static QString classHierarchy()
		{
			auto metaObjectBegin = Obj::staticMetaObject;
			QStringList classHierarchy = { metaObjectBegin.className() };

			for (auto metaObject = metaObjectBegin.superClass(); metaObject; metaObject = metaObject->superClass())
				classHierarchy.push_front(metaObject->className());
			return classHierarchy.join("::");
		}

		static QIcon icon()
		{
			return QIcon();
		}

		static LibraryObject::Constructor constructor()
		{
			return []() { return new Object<Obj>(); };
		}

		static QPair<QString, LibraryObject::FunctionDrag> functionDrag()
		{
			return QPair<QString, LibraryObject::FunctionDrag>();
		}

	private:
	};
}
