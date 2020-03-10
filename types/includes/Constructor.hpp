/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Constructor
*/

#pragma once

#include "IType.hpp"
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QString>

namespace types {
	class Constructor : public IType {
		Q_OBJECT

	public:
		Constructor();
		Constructor(const QMetaMethod &metaMethod);
		Constructor(const Constructor &constructor);
		~Constructor() = default;

		Constructor &operator=(const Constructor &constructor);

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		bool isValid() const;

		QString signature() const;

		QMetaMethod::Access access() const { return _access; }
		void setAccess(QMetaMethod::Access access) { setValue(_access, access, std::bind(&Constructor::accessChanged, this, _access)); }

		QString name() const { return _name; }
		void setName(const QString &name) { setValue(_name, name, std::bind(&Constructor::nameChanged, this, _name)); }

		QList<QPair<QMetaType::Type, QString>> parameters() const { return _parameters; }
		QPair<QMetaType::Type, QString> parameter(int index) const { return _parameters[index]; }
		void setParameters(const QList<QPair<QMetaType::Type, QString>> &parameters) { setValue(_parameters, parameters, std::bind(&Constructor::parametersChanged, this, _parameters)); }
		bool addParameter(QMetaType::Type parameterType, const QString &parameterName);
		bool insertParameter(int index, QMetaType::Type parameterType, const QString &parameterName);
		bool modifyParameterType(int index, QMetaType::Type parameterType);
		bool modifyParameterName(int index, const QString &parameterName);
		void removeParameter(int index);

	signals:
		void accessChanged(QMetaMethod::Access access);
		void nameChanged(const QString &name);
		void parametersChanged(const QList<QPair<QMetaType::Type, QString>> &parameters);

	private:
		QMetaMethod::Access _access;
		QString _name;
		QList<QPair<QMetaType::Type, QString>> _parameters;
	};
}

QDebug operator<<(QDebug debug, const types::Constructor &constructor);
