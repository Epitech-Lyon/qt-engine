/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Exporter
*/

#pragma once

#include <QtCore/QThread>
#include <QtCore/QString>

namespace libraryObjects {
	class Exporter : public QThread {
		Q_OBJECT

	public:
		Exporter(const QString &exportedDirPath, bool generateMain, const QStringList &views);
		~Exporter();

		void run() override;

	signals:
		void currentViewExportedChanged(int index);

	private:
		QString _exportedDirPath;
		bool _generateMain;
		QStringList _views;
	};
}
