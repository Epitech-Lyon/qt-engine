/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Exporter
*/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

namespace libraryObjects {
	class Exporter : public QObject {
		Q_OBJECT

	public:
		Exporter(const QString &exportedDirPath, bool generateMain, const QStringList &views);
		~Exporter();

		QStringList views() const { return _views; }

		void exportProject();

	signals:
		void currentViewExportedChanged(int index);
		void finished();

	private:
		QString _exportedDirPath;
		bool _generateMain;
		QStringList _views;
	};
}
