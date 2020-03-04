/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Exporter
*/

#include "moc_Exporter.cpp"
#include "Exporter.hpp"

#include <QtCore/QDebug>

libraryObjects::Exporter::Exporter(const QString &exportedDirPath, bool generateMain, const QStringList &views)
	: _exportedDirPath(exportedDirPath)
	, _generateMain(generateMain)
	, _views(views)
{
}

libraryObjects::Exporter::~Exporter()
{
}

void libraryObjects::Exporter::run()
{
	qDebug() << "Export" << _views << "to" << _exportedDirPath << (_generateMain ? "with main" : "without main");
	for (int i = 0; i < _views.count(); i += 1) {
		emit currentViewExportedChanged(i);
		sleep(1);
	}
}
