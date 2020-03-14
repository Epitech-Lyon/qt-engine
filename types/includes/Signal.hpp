/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Signal
*/

#pragma once

#include "ClassType.hpp"
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaType>
#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QString>

namespace types {
	class Signal : public ClassType {
		Q_OBJECT

	public:
		Signal();
		Signal(const QMetaMethod &metaMethod);
		~Signal() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		QWidget *initEditor() override;

		bool isValid() const override;
		QString signature() const override;

		QString name() const { return _name; }
		void setName(const QString &name) { setValue(_name, name, std::bind(&Signal::nameChanged, this, _name)); }

		QList<QPair<QMetaType::Type, QString>> parameters() const { return _parameters; }
		QPair<QMetaType::Type, QString> parameter(int index) const { return _parameters[index]; }
		void setParameters(const QList<QPair<QMetaType::Type, QString>> &parameters) { setValue(_parameters, parameters, std::bind(&Signal::parametersChanged, this, _parameters)); }
		bool addParameter(QMetaType::Type parameterType, const QString &parameterName);
		bool modifyParameterType(int index, QMetaType::Type parameterType);
		bool modifyParameterName(int index, const QString &parameterName);
		void removeParameter();

	signals:
		void nameChanged(const QString &name);
		void parametersChanged(const QList<QPair<QMetaType::Type, QString>> &parameters);

	private:
		QString _name;
		QList<QPair<QMetaType::Type, QString>> _parameters;
	};
}

QDebug operator<<(QDebug debug, const types::Signal &signal);
QDebug operator<<(QDebug debug, const types::Signal *signal);
