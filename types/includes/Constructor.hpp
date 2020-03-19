/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Constructor
*/

#pragma once

#include "ClassType.hpp"
#include <QtCore/QMetaMethod>
#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QString>

class QtProperty;

namespace types {
	class Constructor : public ClassType {
		Q_OBJECT

	public:
		Constructor();
		Constructor(const QMetaMethod &metaMethod);
		~Constructor() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		QWidget *initEditor() override;

		bool isValid() const override;
		QString signature() const override;

		QString className() const { return _className; }
		void setClassName(const QString &className) { setValue(_className, className, std::bind(&Constructor::classNameChanged, this, _className)); }

		QList<QPair<QString, QString>> parameters() const { return _parameters; }
		QPair<QString, QString> parameter(int index) const { return _parameters[index]; }
		void setParameters(const QList<QPair<QString, QString>> &parameters) { setValue(_parameters, parameters, std::bind(&Constructor::parametersChanged, this, _parameters)); }
		bool addParameter(const QString &parameterType, const QString &parameterName);
		bool modifyParameterType(int index, const QString &parameterType);
		bool modifyParameterName(int index, const QString &parameterName);
		void removeParameter();

	signals:
		void classNameChanged(const QString &name);
		void parametersChanged(const QList<QPair<QString, QString>> &parameters);

	private:
		QString _className;
		QList<QPair<QString, QString>> _parameters;
	};
}

QDebug operator<<(QDebug debug, const types::Constructor &constructor);
QDebug operator<<(QDebug debug, const types::Constructor *constructor);
