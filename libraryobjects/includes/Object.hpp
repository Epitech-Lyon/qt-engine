/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Object
*/

#pragma once

#include "AObject.hpp"

#include <QtGui/QIcon>

namespace libraryObjects {
	class LibraryFunction;

	template <typename Obj> class Object : public AObject {
	public:
		Object()
			: AObject(new Obj, classHierarchy())
		{
		}

		~Object() = default;

		static void init(AObject *)
		{
		}

		static QJsonObject serializeData(AObject *)
		{
			return QJsonObject();
		}

		static void deserializeData(const QJsonObject &, AObject *)
		{
		}

		static QString classIncludePath()
		{
			return "";
		}

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

		static LibraryFunction *libraryFunction()
		{
			return nullptr;
		}
	};
}
