/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassTypeManager
*/

#include "ClassTypeManager.hpp"

types::ClassTypeManager *types::ClassTypeManager::instance()
{
	static ClassTypeManager classTypeManager;

	return &classTypeManager;
}

QString types::ClassTypeManager::type(QMetaType::Type type) const
{
	return type <= QMetaType::UnknownType ? "" : _types.key(type);
}

void types::ClassTypeManager::registerType(QMetaType::Type type)
{
	if (type <= QMetaType::UnknownType) { return; }

	_types[QMetaType::typeName(type)] = type;
}

void types::ClassTypeManager::unregisterType(QMetaType::Type type)
{
	if (type <= QMetaType::UnknownType) { return; }

	_types.key(type);
}

void types::ClassTypeManager::registerType(const QString &type)
{
	_types[type] = QMetaType::UnknownType;
}

void types::ClassTypeManager::unregisterType(const QString &type)
{
	_types.remove(type);
}
