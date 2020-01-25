/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** MainWindow
*/

#pragma once

#include <QtWidgets/QMainWindow>

namespace qtengine {
	class Manager;
	class MainWindow : public QMainWindow {
	public:
		MainWindow(Manager *);
		~MainWindow() = default;

	private:
		void initMenuBar();
		void initInterface();
		Manager *_manager;

	private slots:
		void onTheme();

	// Layout Management
	private:
		QAction *createLayoutAction(const QString &);
		QAction *getLayoutActionByName(const QString &) const;
		QList<QAction *> getLayoutsAction() const;
		QStringList getLayoutsActionName() const;
		QMenu *_menuLayouts;
		QAction *_menuLayoutsSeparator;
		QAction *_menuLayoutsBtnDelete;

	private slots:
		void onSaveLayout();
		void onDeleteLayout();
	};
}
