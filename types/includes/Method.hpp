/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Method
*/

#pragma once

#include "ClassType.hpp"
#include <QtCore/QMetaMethod>
#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QString>

namespace types {
	class Method : public ClassType {
		Q_OBJECT

	public:
		Method();
		Method(const QMetaMethod &metaMethod);
		~Method() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		QWidget *initEditor() override;

		bool isValid() const override;
		QString signature() const override;

		bool isStatic() const { return _isStatic; }
		void setStatic(bool isStatic) { setValue(_isStatic, isStatic, std::bind(&Method::isStaticChanged, this, _isStatic)); }

		QString returnType() const { return _returnType; }
		void setReturnType(const QString &returnType) { setValue(_returnType, returnType, std::bind(&Method::returnTypeChanged, this, _returnType)); }

		QString name() const { return _name; }
		void setName(const QString &name) { setValue(_name, name, std::bind(&Method::nameChanged, this, _name)); }

		QList<QPair<QString, QString>> parameters() const { return _parameters; }
		QPair<QString, QString> parameter(int index) const { return _parameters[index]; }
		void setParameters(const QList<QPair<QString, QString>> &parameters) { setValue(_parameters, parameters, std::bind(&Method::parametersChanged, this, _parameters)); }
		bool addParameter(const QString &parameterType, const QString &parameterName);
		bool modifyParameterType(int index, const QString &parameterType);
		bool modifyParameterName(int index, const QString &parameterName);
		void removeParameter();

		bool isConst() const { return _isConst; }
		void setConst(bool isConst) { setValue(_isConst, isConst, std::bind(&Method::isConstChanged, this, _isConst)); }

	signals:
		void isStaticChanged(bool isStatic);
		void returnTypeChanged(QString type);
		void nameChanged(const QString &name);
		void parametersChanged(const QList<QPair<QString, QString>> &parameters);
		void isConstChanged(bool isConst);

	private:
		bool _isStatic;
		QString _returnType;
		QString _name;
		QList<QPair<QString, QString>> _parameters;
		bool _isConst;
	};
}

QDebug operator<<(QDebug debug, const types::Method &method);
QDebug operator<<(QDebug debug, const types::Method *method);
