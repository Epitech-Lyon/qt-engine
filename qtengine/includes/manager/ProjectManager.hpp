/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ProjectManager
*/

#pragma once

#include <QtCore/QObject>
#include "ISerializable.hpp"
#include <QtCore/QString>

namespace libraryObjects {
	class LibraryObject;
}

namespace qtengine {
	class ProjectManager : public QObject, public types::ISerializable {
		Q_OBJECT

	// General
	public:
		ProjectManager() = default;
		~ProjectManager() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		void openProject(const QString &projectPath);

		bool projectIsOpened() const { return _projectIsOpened; }
		QString projectPath() const { return _projectPath; }
		QString projectName() const { return _projectName; }
		QStringList recentsProject() const { return _recentsProject; }

	signals:
		void projectOpened(bool isOpened);
		void projectPathChanged(const QString &projectPath);
		void projectNameChanged(const QString &projectName);
		void recentProjectsChanged(const QStringList &views);

	public slots:
		void onNewProject();
		void onOpenProject();
		void onSaveProject();
		void onExportProject();
		void onCloseProject();

	private:
		const QString _projectExt = ".prj";
		const int _maxRecentsProject = 5;
		bool _projectIsOpened = false;
		QString _projectPath;
		QString _projectName;
		QStringList _recentsProject;

	// Views management
	public:
		QStringList views() const { return _views; }

	public slots:
		void onCreateView();
		void onImportView();
		void onRemoveView(const QString &viewPath);

	signals:
		void viewsChanged(const QStringList &viewsPath);

	private:
		void emitViewsChanged();
		QStringList _views;
	};
}
