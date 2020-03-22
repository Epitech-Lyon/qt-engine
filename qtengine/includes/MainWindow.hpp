/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** MainWindow
*/

#pragma once

#include <QtWidgets/QMainWindow>
#include "ISerializable.hpp"

namespace qtengine {
	class Manager;
	class MainWindow : public QMainWindow, public types::ISerializable {
	public:
		MainWindow(Manager *);
		~MainWindow();

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

		void setTitleProject(const QString &titleProject);
		void setTitleView(const QString &titleView);
		void setTitleClassType(const QString &titleClassType);

	private:
		void initMenuBar();
		void initInterface();
		void refreshTitle();
		Manager *_manager;
		QString _titleProject;
		QString _titleView;
		QString _titleClassType;
	};
}
