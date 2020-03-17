/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelLibraryObject
*/

#include "ContentPanelLibraryObject.hpp"

#include "TreeWidgetLibraryObject.hpp"
#include "LibraryObjectManager.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"

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

	auto onViewChanged = [this](bool isOpened) {
		_tree->setEnabled(isOpened);
	};

	onViewChanged(Manager::instance()->viewManager()->viewIsOpened());
	connect(Manager::instance()->viewManager(), &ViewManager::viewOpened, this, onViewChanged);
}
