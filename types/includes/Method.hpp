/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Method
*/

#pragma once

#include "IType.hpp"
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QString>

namespace types {
	class Method : public IType {
		Q_OBJECT

	public:
		Method();
		Method(const QMetaMethod &metaMethod);
		Method(const Method &method);
		~Method() = default;

		Method &operator=(const Method &method);
		bool operator==(const Method &method);

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		bool isValid() const;

		QString signature() const;

		QMetaMethod::Access access() const { return _access; }
		void setAccess(QMetaMethod::Access access) { setValue(_access, access, std::bind(&Method::accessChanged, this, _access)); }

		bool isStatic() const { return _isStatic; }
		void setStatic(bool isStatic) { setValue(_isStatic, isStatic, std::bind(&Method::isStaticChanged, this, _isStatic)); }

		QMetaType::Type returnType() const { return _returnType; }
		void setReturnType(QMetaType::Type returnType) { setValue(_returnType, returnType, std::bind(&Method::returnTypeChanged, this, _returnType)); }

		QString name() const { return _name; }
		void setName(const QString &name) { setValue(_name, name, std::bind(&Method::nameChanged, this, _name)); }

		QList<QPair<QMetaType::Type, QString>> parameters() const { return _parameters; }
		QPair<QMetaType::Type, QString> parameter(int index) const { return _parameters[index]; }
		void setParameters(const QList<QPair<QMetaType::Type, QString>> &parameters) { setValue(_parameters, parameters, std::bind(&Method::parametersChanged, this, _parameters)); }
		bool addParameter(QMetaType::Type parameterType, const QString &parameterName);
		bool insertParameter(int index, QMetaType::Type parameterType, const QString &parameterName);
		bool modifyParameterType(int index, QMetaType::Type parameterType);
		bool modifyParameterName(int index, const QString &parameterName);
		void removeParameter(int index);

		bool isConst() const { return _isConst; }
		void setConst(bool isConst) { setValue(_isConst, isConst, std::bind(&Method::isConstChanged, this, _isConst)); }

	signals:
		void accessChanged(QMetaMethod::Access access);
		void isStaticChanged(bool isStatic);
		void returnTypeChanged(QMetaType::Type type);
		void nameChanged(const QString &name);
		void parametersChanged(const QList<QPair<QMetaType::Type, QString>> &parameters);
		void isConstChanged(bool isConst);

	private:
		QMetaMethod::Access _access;
		bool _isStatic;
		QMetaType::Type _returnType;
		QString _name;
		QList<QPair<QMetaType::Type, QString>> _parameters;
		bool _isConst;
	};
}

QDebug operator<<(QDebug debug, const types::Method &method);
QDebug operator<<(QDebug debug, const types::Method *method);
