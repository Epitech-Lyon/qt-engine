/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ProjectManager
*/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include "ISerializable.hpp"

namespace qtengine {
	class ProjectManager : public QObject, public ISerializable {
		Q_OBJECT

	public:
		ProjectManager() = default;
		~ProjectManager() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

		void openProject(const QString &);

		QString projectDir() const { return _projectDir; }
		QString projectPath() const { return _projectPath; }
		QString projectName() const { return _projectName; }
		QStringList recentsProject() const { return _recentsProject; }

	signals:
		void projectChanged();
		void projectDirChanged(const QString &);
		void projectPathChanged(const QString &);
		void projectNameChanged(const QString &);
		void recentProjectsChanged(const QStringList &);

	public slots:
		void onNewProject();
		void onOpenProject();
		void onExportProject();

	private:
		const QString _projectExt = ".prj";
		QString _projectDir;
		QString _projectPath;
		QString _projectName;
		const int _maxRecentsProject = 5;
		QStringList _recentsProject;
	};
}
