/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelLibraryObject
*/

#include "ContentPanelLibraryObject.hpp"

#include "TreeWidgetLibraryObject.hpp"
#include "LibraryObjectManager.hpp"

qtengine::ContentPanelLibraryObject::ContentPanelLibraryObject(QWidget *parent)
	: ContentPanelBase("Library Object", parent)
{
}

void qtengine::ContentPanelLibraryObject::init()
{
	_tree = new TreeWidgetLibraryObject(this);
	_mainLayout->addWidget(_tree);

	for (auto libraryObject : libraryObjects::LibraryObjectManager::instance()->libraryObjects())
		_tree->addLibraryObject(libraryObject);
	_tree->expandAll();
}
