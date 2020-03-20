/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassTypeExporter
*/

#include "ClassTypeExporter.hpp"

#include "ClassType.hpp"

#include <QtCore/QDebug>

libraryObjects::ClassTypeExporter::ClassTypeExporter(const QJsonObject &json)
	: _classType(types::ClassType::construct(static_cast<types::ClassType::Type>(json["type"].toInt())))
{
	_classType->deserialize(json);
	if (!_classType->content().isEmpty())
		build();
}

libraryObjects::ClassTypeExporter::~ClassTypeExporter()
{
	delete _classType;
}

QString libraryObjects::ClassTypeExporter::signature() const
{
	return _classType->signature();
}

void libraryObjects::ClassTypeExporter::build()
{
}
