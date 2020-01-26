/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** main
*/

#include <QtWidgets/QApplication>
#include "Manager.hpp"
#include "MainWindow.hpp"

int main(int ac, char **av)
{
    QApplication app(ac, av);
    auto manager = qtengine::Manager::instance();

    manager->init();
    manager->mainWindow()->showMaximized();
	return app.exec();
}
