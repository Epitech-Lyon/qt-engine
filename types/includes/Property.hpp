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
		bool isUserType() const { return _userType; }

		QMetaMethod::Access access() const { return _access; }
		void setAccess(QMetaMethod::Access access) { setValue(_access, access, std::bind(&Property::accessChanged, this, _access)); }

		QMetaType::Type type() const { return _type; }
		void setType(QMetaType::Type type) { setValue(_type, type, std::bind(&Property::typeChanged, this, _type)); }

		QString name() const { return _name; }
		void setName(const QString &name) { setValue(_name, name, std::bind(&Property::nameChanged, this, _name)); }

		QString setterSignature() const;
		QString setterName() const { return _setterName; }
		void setSetterName(const QString &setterName) { setValue(_setterName, setterName, std::bind(&Property::setterNameChanged, this, _setterName)); }

		QString getterSignature() const;
		QString getterName() const { return _getterName; }
		void setGetterName(const QString &getterName) { setValue(_getterName, getterName, std::bind(&Property::getterNameChanged, this, _getterName)); }

	signals:
		void accessChanged(QMetaMethod::Access access);
		void typeChanged(QMetaType::Type type);
		void nameChanged(const QString &name);
		void setterNameChanged(const QString &setterName);
		void getterNameChanged(const QString &getterName);

	private:
		bool _userType;
		QMetaMethod::Access _access;
		QMetaType::Type _type;
		QString _name;
		QString _setterName;
		QString _getterName;
	};
}

QDebug operator<<(QDebug debug, const types::Property &property);
