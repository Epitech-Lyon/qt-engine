/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObjectManager
*/

#include "LibraryObjectManager.hpp"

#include "Object.hpp"
#include <QtWidgets>
libraryObjects::LibraryObjectManager::LibraryObjectManager()
{
	registerObject<Object<QWidget>>();
	registerObject<Object<QVBoxLayout>>();
	registerObject<Object<QHBoxLayout>>();
}

libraryObjects::LibraryObjectManager *libraryObjects::LibraryObjectManager::instance()
{
	static LibraryObjectManager libraryObjectManager;

	return &libraryObjectManager;
}
