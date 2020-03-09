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

	private:
		void initMenuBar();
		void initInterface();
		Manager *_manager;
	};
}
