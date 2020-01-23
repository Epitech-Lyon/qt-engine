/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** main
*/

#include <QtWidgets/QApplication>
#include "MainWindow.hpp"

int main(int ac, char **av)
{
    QApplication app(ac, av);
    qtengine::MainWindow mainWindow;

    mainWindow.showMaximized();
	return app.exec();
}
