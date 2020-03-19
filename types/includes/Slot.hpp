/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Slot
*/

#pragma once

#include "ClassType.hpp"
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QString>

namespace types {
	class Slot : public ClassType {
		Q_OBJECT

	public:
		Slot();
		Slot(const QMetaMethod &metaMethod);
		~Slot() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		QWidget *initEditor() override;

		bool isValid() const override;
		QString signature() const override;

		QString name() const { return _name; }
		void setName(const QString &name) { setValue(_name, name, std::bind(&Slot::nameChanged, this, _name)); }

		QList<QPair<QString, QString>> parameters() const { return _parameters; }
		QPair<QString, QString> parameter(int index) const { return _parameters[index]; }
		void setParameters(const QList<QPair<QString, QString>> &parameters) { setValue(_parameters, parameters, std::bind(&Slot::parametersChanged, this, _parameters)); }
		bool addParameter(const QString &parameterType, const QString &parameterName);
		bool modifyParameterType(int index, const QString &parameterType);
		bool modifyParameterName(int index, const QString &parameterName);
		void removeParameter();

		bool isConst() const { return _isConst; }
		void setConst(bool isConst) { setValue(_isConst, isConst, std::bind(&Slot::isConstChanged, this, _isConst)); }

	signals:
		void nameChanged(const QString &name);
		void parametersChanged(const QList<QPair<QString, QString>> &parameters);
		void isConstChanged(bool isConst);

	private:
		QString _name;
		QList<QPair<QString, QString>> _parameters;
		bool _isConst;
	};
}

QDebug operator<<(QDebug debug, const types::Slot &slot);
QDebug operator<<(QDebug debug, const types::Slot *slot);
