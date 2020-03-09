/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Property
*/

#pragma once

#include "IType.hpp"
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <QtCore/QString>

namespace types {
	class Property : public IType {
		Q_OBJECT

	public:
		Property();
		Property(const QMetaProperty &metaProperty);
		Property(const Property &property);
		~Property() = default;

		Property &operator=(const Property &property);

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		bool isValid() const { return !_name.isEmpty(); }

		QMetaMethod::Access access() const { return _access; }
		void setAccess(QMetaMethod::Access access) { setValue(_access, access, std::bind(&Property::accessChanged, this, _access)); }

		QMetaType::Type type() const { return _type; }
		void setType(QMetaType::Type type) { setValue(_type, type, std::bind(&Property::typeChanged, this, _type)); }

		QString name() const { return _name; }
		void setName(const QString &name) { setValue(_name, name, std::bind(&Property::nameChanged, this, _name)); }

		bool haveSetter() const { return _haveSetter; }
		void haveSetter(bool haveSetter) { setValue(_haveSetter, haveSetter, std::bind(&Property::haveSetterChanged, this, _haveSetter)); }

		bool haveGetter() const { return _haveGetter; }
		void haveGetter(bool haveGetter) { setValue(_haveGetter, haveGetter, std::bind(&Property::haveGetterChanged, this, _haveGetter)); }

	signals:
		void accessChanged(QMetaMethod::Access access);
		void typeChanged(QMetaType::Type type);
		void nameChanged(const QString &name);
		void haveSetterChanged(bool);
		void haveGetterChanged(bool);

	private:
		QMetaMethod::Access _access;
		QMetaType::Type _type;
		QString _name;
		bool _haveSetter;
		bool _haveGetter;
	};
}

QDebug operator<<(QDebug debug, const types::Property &property);
