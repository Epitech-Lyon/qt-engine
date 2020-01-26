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
	class ProjectManager;
	class LayoutManager;

	class Manager : public QObject {
	public:
		~Manager();
		static Manager *instance();
		void init();
		void save();

		MainWindow *mainWindow() const { return _mainWindow; }
		ProjectManager *projectManager() const { return _projectManager; }
		LayoutManager *layoutManager() const { return _layoutManager; }

	private:
		Manager();
		ProjectManager *_projectManager;
		LayoutManager *_layoutManager;
		MainWindow *_mainWindow;

	// Theme
	public slots:
		void onTheme();
	
	private:
		QString _theme;
	};
}
