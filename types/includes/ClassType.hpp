/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassType
*/

#pragma once

#include <QtCore/QObject>
#include "ISerializable.hpp"

#include <QtCore/QList>
#include <QtCore/QMetaMethod>
#include <QtWidgets/QWidget>

namespace types {
	class ClassType : public QObject, public ISerializable {
		Q_OBJECT

	public:
		enum Type {
			CONSTRUCTOR,
			METHOD,
			SIGNAL,
			SLOT,
			PROPERTY
		};
		Q_ENUM(Type)

		static ClassType *construct(Type type);

		virtual ~ClassType() = default;

		virtual QWidget *initEditor() = 0;

		virtual bool isValid() const = 0;
		virtual QString signature() const = 0;

		QMetaMethod::Access access() const { return _access; }
		void setAccess(QMetaMethod::Access access) { setValue(_access, access, std::bind(&ClassType::accessChanged, this, _access)); }

		Type type() const { return _type; }
		bool isType(Type type) const { return _type == type; }

	signals:
		void accessChanged(QMetaMethod::Access access);

	protected:
		ClassType(QMetaMethod::Access access, Type type);

		template<typename T> inline void setValue(T &value, const T &newValue, std::function<void ()> emitFunction)
		{
			if (value != newValue) {
				value = newValue;
				emitFunction();
			}
		}

		QMetaMethod::Access _access;
		Type _type;
	};
}
