/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassTypeManager
*/

#pragma once

#include <QtCore/QMap>
#include <QtCore/QMetaType>

namespace types {
	class ClassTypeManager {
	public:
		~ClassTypeManager() = default;
		static ClassTypeManager *instance();

		QString type(QMetaType::Type type) const;
		QStringList types() const { return _types.keys(); }
		bool isCustomType(const QString &type) const { return _types[type] == QMetaType::UnknownType; }
		QMetaType::Type typeValue(const QString &type) const { return _types[type]; }

		void registerType(QMetaType::Type type);
		void unregisterType(QMetaType::Type type);

		void registerType(const QString &type);
		void unregisterType(const QString &type);

	private:
		ClassTypeManager() = default;
		QMap<QString, QMetaType::Type> _types;
	};
}
