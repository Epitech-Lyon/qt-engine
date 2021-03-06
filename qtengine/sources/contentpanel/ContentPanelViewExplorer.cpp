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
#include "LibraryFunction.hpp"

#include <QtGui/QDragMoveEvent>
#include <QtWidgets/QMenu>

qtengine::TreeViewExplorer::TreeViewExplorer(QWidget *parent)
	: QTreeWidget(parent)
	, _itemEditorOpened(nullptr)
{
	setRootIsDecorated(true);
	setAnimated(true);
	setDragDropMode(QAbstractItemView::DropOnly);
	setDropIndicatorShown(true);
	setHeaderLabels({"Object name", "Class"});
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, &QTreeWidget::itemClicked, [this](QTreeWidgetItem *item, int) {
		if (item != _itemEditorOpened) {
			closePersistentEditor(_itemEditorOpened, 0);
			_itemEditorOpened = nullptr;
		}
		emit objectClicked(_objects[item]);
	});
	connect(this, &QTreeWidget::itemDoubleClicked, [this](QTreeWidgetItem *item, int column)
	{
		if (column == 0) {
			_itemEditorOpened = item;
			openPersistentEditor(item, 0);
		}
	});
	connect(this, &QTreeWidget::itemChanged, [this](QTreeWidgetItem *item, int)
	{
		if (isPersistentEditorOpen(item, 0)) {
			closePersistentEditor(item, 0);
			_itemEditorOpened = nullptr;
			_objects[item]->setObjectName(item->text(0));
			item->setText(0, _objects[item]->objectName());
		}
	});
	connect(this, &QTreeWidget::customContextMenuRequested, [this](const QPoint &pos) {
		auto objectItem = itemAt(pos);
		if (!objectItem) { return; }

		emit openMenuFor(_objects[objectItem], _objects[objectItem->parent()], mapToGlobal(pos));
	});
}

QTreeWidgetItem *qtengine::TreeViewExplorer::createItemFor(libraryObjects::AObject *object, QTreeWidgetItem *parent, bool recursively, int index)
{
	if (!object) { return nullptr; }
	auto item = new QTreeWidgetItem({object->objectName(), object->className()});

	parent->insertChild(index == -1 ? parent->childCount() : index, item);
	_objects[item] = object;
	connect(object, &libraryObjects::AObject::propertyUpdated, this, &TreeViewExplorer::onPropertyUpdated);
	if (recursively)
		for (auto objectChild : object->children())
			createItemFor(objectChild, item, true, -1);
	return item;
}

QTreeWidgetItem *qtengine::TreeViewExplorer::createItemFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, bool recursively, int index)
{
	if (!object || !parent) { return nullptr; }
	return createItemFor(object, _objects.key(parent), recursively, index);
}

void qtengine::TreeViewExplorer::setCurrentItemFor(libraryObjects::AObject *object)
{
	if (!object) { return; }
	setCurrentItem(_objects.key(object));
}

void qtengine::TreeViewExplorer::removeItemFor(libraryObjects::AObject *object)
{
	if (!object) { return; }
	auto item = _objects.key(object);

	delete item;
	_objects.remove(item);
	disconnect(object, &libraryObjects::AObject::propertyUpdated, this, &TreeViewExplorer::onPropertyUpdated);
}

void qtengine::TreeViewExplorer::dragMoveEvent(QDragMoveEvent *event)
{
	QTreeWidget::dragMoveEvent(event);
	auto libraryObjectMimeData = dynamic_cast<const libraryObjects::LibraryObjectMimeData *>(event->mimeData());
	if (!libraryObjectMimeData) { return; }

	auto item = itemAt(event->pos());
	if (!_objects[item]) { return; }

	switch (dropIndicatorPosition()) {
	case QTreeWidget::OnItem:
		break;
	case QTreeWidget::AboveItem:
		item = item->parent();
		break;
	case QTreeWidget::BelowItem:
		item = item->parent();
		break;
	case QTreeWidget::OnViewport:
		break;
	}
	if (!_objects[item]) { return; }

	auto libraryObjectParent = libraryObjects::LibraryObjectManager::instance()->libraryObjectOf(_objects[item]->classHierarchy());
	if (!libraryObjectParent->libraryFunction()->functionDragFor(libraryObjectMimeData->libraryObject()->classHierarchy()).isValid)
		event->ignore();
}

bool qtengine::TreeViewExplorer::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction)
{
	auto libraryObjectMimeData = dynamic_cast<const libraryObjects::LibraryObjectMimeData *>(data);
	if (!libraryObjectMimeData) { return false; }

	emit libraryObjectDropped(_objects[parent], index, libraryObjectMimeData->libraryObject());
	return false;
}

void qtengine::TreeViewExplorer::onPropertyUpdated(const QString &propertyName, const QVariant &propertyValue)
{
	if (propertyName == "objectName")
		_objects.key(dynamic_cast<libraryObjects::AObject*>(sender()))->setText(0, propertyValue.toString());
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
	connect(Manager::instance()->viewManager(), &ViewManager::currentObjectChanged, _tree, &TreeViewExplorer::setCurrentItemFor);

	connect(_tree, &TreeViewExplorer::objectClicked, Manager::instance()->viewManager(), &ViewManager::setCurrentObject);
	connect(_tree, &TreeViewExplorer::openMenuFor, this, &ContentPanelViewExplorer::onOpenMenuFor);
	connect(_tree, &TreeViewExplorer::libraryObjectDropped, this, &ContentPanelViewExplorer::onLibraryObjectDropped);
}

void qtengine::ContentPanelViewExplorer::onViewObjectChanged(libraryObjects::AObject *viewObject)
{
	_tree->clear();
	_tree->invisibleRootItem()->setFlags(viewObject || Manager::instance()->viewManager()->viewName().isEmpty() ? Qt::NoItemFlags : Qt::ItemIsDropEnabled);
	_tree->createItemFor(viewObject, _tree->invisibleRootItem());
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

void qtengine::ContentPanelViewExplorer::onLibraryObjectDropped(libraryObjects::AObject *parent, int index, libraryObjects::LibraryObject *child)
{
	if (!parent) {
		auto viewObject = child->constructor();

		if (!Manager::instance()->viewManager()->createView(viewObject))
			delete viewObject;
	} else {
		auto libraryObject = libraryObjects::LibraryObjectManager::instance()->libraryObjectOf(parent->classHierarchy());
		if (!libraryObject) { return; }

		auto function = libraryObject->libraryFunction()->functionDragFor(child->classHierarchy());
		if (!function.isValid) { return; }

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
