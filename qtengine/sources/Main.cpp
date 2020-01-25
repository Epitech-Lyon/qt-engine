/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** main
*/

#include <QtWidgets/QApplication>
#include "Manager.hpp"

int main(int ac, char **av)
{
    QApplication app(ac, av);
    qtengine::Manager::instance();

	return app.exec();
}
