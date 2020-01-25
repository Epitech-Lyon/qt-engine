/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Manager
*/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

namespace qtengine {
	class MainWindow;

	class Manager : public QObject {
		Q_OBJECT
	public:
		~Manager();
		static Manager *instance();

	private:
		Manager();
		MainWindow *_mainWindow;

	// Project management
	public:
		QString projectDir() const { return _projectDir; }
		QString projectPath() const { return _projectPath; }
		QString projectName() const { return _projectName; }

	signals:
		void projectChanged();
		void projectDirChanged(const QString &);
		void projectPathChanged(const QString &);
		void projectNameChanged(const QString &);

	public slots:
		void onNewProject();
		void onOpenProject();
		void onSaveProject();
		void onSaveProjectAs();

	private:
		void openProject(const QString &);
		const QString _projectExt = ".prj";
		QString _projectDir;
		QString _projectPath;
		QString _projectName;
	};
}
