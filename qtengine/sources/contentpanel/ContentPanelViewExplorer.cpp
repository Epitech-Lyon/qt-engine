/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewExplorer
*/

#include "moc_ContentPanelViewExplorer.cpp"
#include "ContentPanelViewExplorer.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"

#include "AObject.hpp"
#include "LibraryObjectManager.hpp"
#include "LibraryObjectMimeData.hpp"
#include "LibraryObject.hpp"

#include <QtGui/QDragMoveEvent>

qtengine::TreeViewExplorer::TreeViewExplorer(QWidget *parent)
	: QTreeWidget(parent)
{
	setRootIsDecorated(true);
	setAnimated(true);
	setDragDropMode(QAbstractItemView::DropOnly);
	setDropIndicatorShown(true);
	setHeaderLabels({"Object name", "Class"});
	invisibleRootItem()->setFlags(invisibleRootItem()->flags() ^ Qt::ItemIsDropEnabled);

	connect(this, &QTreeWidget::itemClicked, [this](QTreeWidgetItem *item, int) { emit objectClicked(_objects[item]); });
}

QTreeWidgetItem *qtengine::TreeViewExplorer::createItemFor(libraryObjects::AObject *object, QTreeWidgetItem *parent, bool recursively)
{
	if (!object) { return nullptr; }
	auto item = new QTreeWidgetItem(parent, {object->objectName(), object->className()});

	_objects[item] = object;
	if (recursively)
		for (auto objectChild : object->children())
			createItemFor(objectChild, item, true);
	return item;
}

QTreeWidgetItem *qtengine::TreeViewExplorer::createItemFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, bool recursively)
{
	if (!object || !parent) { return nullptr; }
	return createItemFor(object, _objects.key(parent), recursively);
}

void qtengine::TreeViewExplorer::dragMoveEvent(QDragMoveEvent *event)
{
	QTreeWidget::dragMoveEvent(event);
	auto libraryObjectMimeData = dynamic_cast<const libraryObjects::LibraryObjectMimeData *>(event->mimeData());
	if (!libraryObjectMimeData) { return; }

	auto item = itemAt(event->pos());
	if (!_objects[item]) { return; }

	auto libraryObjectParent = libraryObjects::LibraryObjectManager::instance()->libraryObjectOf(_objects[item]->classHierarchy());
	if (!libraryObjectParent->canCallFunctionDrag(libraryObjectMimeData->libraryObject()))
		event->ignore();
}

bool qtengine::TreeViewExplorer::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction)
{
	auto libraryObjectMimeData = dynamic_cast<const libraryObjects::LibraryObjectMimeData *>(data);
	if (!libraryObjectMimeData) { return false; }

	emit libraryObjectDropped(_objects[parent], index, libraryObjectMimeData->libraryObject());
	return false;
}

qtengine::ContentPanelViewExplorer::ContentPanelViewExplorer(QWidget *parent)
	: ContentPanelBase("View Explorer", parent)
{
}

void qtengine::ContentPanelViewExplorer::init()
{
	_tree = new TreeViewExplorer(this);
	_tree->setHeaderLabels({"Object name", "Class"});
	_mainLayout->addWidget(_tree);

	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelViewExplorer::onViewObjectChanged);

	connect(_tree, &TreeViewExplorer::objectClicked, Manager::instance()->viewManager(), &ViewManager::setCurrentObject);
	connect(_tree, &TreeViewExplorer::libraryObjectDropped, this, &ContentPanelViewExplorer::onLibraryObjectDropped);
}

void qtengine::ContentPanelViewExplorer::onViewObjectChanged(libraryObjects::AObject *viewObject)
{
	_tree->clear();
	_tree->createItemFor(viewObject, _tree->invisibleRootItem());
	_tree->expandAll();
}

void qtengine::ContentPanelViewExplorer::onLibraryObjectDropped(libraryObjects::AObject *parent, int position, libraryObjects::LibraryObject *child)
{
	auto libraryObject = libraryObjects::LibraryObjectManager::instance()->libraryObjectOf(parent->classHierarchy());
	if (!libraryObject) { return; }

	auto objectChild = libraryObject->functionDrag(parent, position, child);
	auto objectChildItem = _tree->createItemFor(objectChild, parent);
	
	if (objectChildItem)
		objectChildItem->parent()->setExpanded(true);
}
