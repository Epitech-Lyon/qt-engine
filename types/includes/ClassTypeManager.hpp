/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassTypeManager
*/

#pragma once

#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QMetaType>

namespace types {
	class ClassTypeManager {
	public:
		typedef struct Function {
			QStringList inputs;
			QString output;
			QString name;
			QString code;
		} Function;

		~ClassTypeManager() = default;
		static ClassTypeManager *instance();

		QString type(QMetaType::Type type) const;
		QStringList types() const { return _types.keys(); }

		bool isCustomType(const QString &type) const { return _types[type] == QMetaType::UnknownType; }
		QMetaType::Type typeValue(const QString &type) const { return _types[type]; }

		QVector<Function> functionsForType(const QString &type) const { return _functions[type]; }

		void registerType(const QString &type);
		void registerFunctionForType(const QString &type, const Function &function);
		void unregisterType(const QString &type);

	private:
		ClassTypeManager();
		void registerType(QMetaType::Type type);
		void registerFunctionOperatorForType(const QString &type, const QChar &characOperator);
		void registerFunctionOperatorConditionalForType(const QString &type, const QString &operatorConditional);
		QMap<QString, QMetaType::Type> _types;
		QMap<QString, QVector<Function>> _functions;
	};
}
