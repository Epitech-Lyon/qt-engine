/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Manager
*/

#include "Manager.hpp"

qtengine::Manager::Manager()
{
}

qtengine::Manager::~Manager()
{
}

qtengine::Manager *qtengine::Manager::instance()
{
	static Manager manager;

	return &manager;
}
