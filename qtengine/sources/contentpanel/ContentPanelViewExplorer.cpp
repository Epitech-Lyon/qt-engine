/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewExplorer
*/

#include "moc_ContentPanelViewExplorer.cpp"
#include "ContentPanelViewExplorer.hpp"

#include "TreeWidgetViewExplorer.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"

#include "AObject.hpp"
#include "LibraryObjectManager.hpp"
#include "LibraryObject.hpp"
#include "LibraryFunction.hpp"

#include <QtWidgets/QMenu>

qtengine::ContentPanelViewExplorer::ContentPanelViewExplorer(QWidget *parent)
	: ContentPanelBase("View Explorer", parent)
{
}

void qtengine::ContentPanelViewExplorer::init()
{
	_tree = new TreeWidgetViewExplorer(this);
	_mainLayout->addWidget(_tree);

	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelViewExplorer::onViewObjectChanged);
	connect(Manager::instance()->viewManager(), &ViewManager::currentObjectChanged, _tree, &TreeWidgetViewExplorer::setCurrentItemFor);

	connect(_tree, &TreeWidgetViewExplorer::objectClicked, Manager::instance()->viewManager(), &ViewManager::setCurrentObject);
	connect(_tree, &TreeWidgetViewExplorer::openMenuFor, this, &ContentPanelViewExplorer::onOpenMenuFor);
	connect(_tree, &TreeWidgetViewExplorer::libraryObjectDropped, this, &ContentPanelViewExplorer::onLibraryObjectDropped);
}

void qtengine::ContentPanelViewExplorer::onViewObjectChanged(libraryObjects::AObject *viewObject)
{
	_tree->clear();

	auto rootItem = _tree->createItemFor(viewObject, _tree->invisibleRootItem());
	if (rootItem)
		rootItem->setFlags(rootItem->flags() ^ Qt::ItemIsDragEnabled);

	_tree->expandAll();
}

void qtengine::ContentPanelViewExplorer::onOpenMenuFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, const QPoint &pos)
{
	QMenu menu;

	if (parent) {
		auto libraryObjectParent = libraryObjects::LibraryObjectManager::instance()->libraryObjectOf(parent->classHierarchy());
		if (libraryObjectParent) {
			auto functionDrag = libraryObjectParent->libraryFunction()->functionDragFor(object->classHierarchy());
			if (functionDrag.isValid) {
				auto callback = [&, this]() {
					if (functionDrag.functionRemove(parent, object)) {
						_tree->removeItemFor(object);
						Manager::instance()->viewManager()->setCurrentObject(parent);
						delete object;
					}
				};
				menu.addAction(functionDrag.functionRemoveName, callback);
			}

			auto functionsMenuChildren = libraryObjectParent->libraryFunction()->functionsMenuChildren();
			if (!menu.isEmpty() && !functionsMenuChildren.isEmpty())
				menu.addSeparator();
			for (auto functionMenuChildren : functionsMenuChildren)
				if (functionMenuChildren.isValid)
					menu.addAction(functionMenuChildren.functionName, std::bind(functionMenuChildren.function, object));
		}
	}

	auto libraryObject = libraryObjects::LibraryObjectManager::instance()->libraryObjectOf(object->classHierarchy());
	if (libraryObject) {
		auto functionsMenuParent = libraryObject->libraryFunction()->functionsMenuParent();
		if (!menu.isEmpty() && !functionsMenuParent.isEmpty())
			menu.addSeparator();
		for (auto functionMenuParent : functionsMenuParent)
			if (functionMenuParent.isValid)
				menu.addAction(functionMenuParent.functionName, std::bind(functionMenuParent.function, object));
	}
	if (!menu.isEmpty())
		menu.exec(pos);
}

void qtengine::ContentPanelViewExplorer::onLibraryObjectDropped(libraryObjects::AObject *parent, int index, libraryObjects::LibraryObject *child, libraryObjects::AObject *reference)
{
	auto libraryObject = libraryObjects::LibraryObjectManager::instance()->libraryObjectOf(parent->classHierarchy());
	if (!libraryObject) { return; }

	auto function = libraryObject->libraryFunction()->functionDragFor(child->classHierarchy());
	if (!function.isValid) { return; }

	if (reference) {
		function.functionRemove(parent, reference);
		auto childItem = _tree->itemFor(reference);
		auto newParentItem = _tree->itemFor(parent);

		auto oldParentItem = childItem->parent();
		auto oldIndex = oldParentItem->indexOfChild(childItem);

		index = newParentItem == oldParentItem && index > oldIndex ? index - 1 : index;
		oldParentItem->removeChild(childItem);

		function.functionAdd(parent, index, reference);
		newParentItem->insertChild(index, childItem);
		newParentItem->setExpanded(true);
		_tree->expandRecursivelyItemFor(reference);
		_tree->setCurrentItem(newParentItem);
		Manager::instance()->viewManager()->setCurrentObject(reference);
	} else {
		auto childObject = child->constructor();
		if (!childObject) { return; }

		if (!function.functionAdd(parent, index, childObject)) { delete childObject; return; }

		auto childObjectItem = _tree->createItemFor(childObject, parent, false, index);
		if (childObjectItem) {
			childObjectItem->parent()->setExpanded(true);
			_tree->setCurrentItem(childObjectItem);
			Manager::instance()->viewManager()->setCurrentObject(childObject);
		}
	}
}
