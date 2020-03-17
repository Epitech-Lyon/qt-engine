/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TreeWidgetViewExplorer
*/

#include "moc_TreeWidgetViewExplorer.cpp"
#include "TreeWidgetViewExplorer.hpp"

#include "AObject.hpp"
#include "LibraryObjectManager.hpp"
#include "LibraryObject.hpp"
#include "LibraryFunction.hpp"
#include "MimeDataObject.hpp"
#include "ObjectClass.hpp"

#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>

qtengine::TreeWidgetViewExplorer::TreeWidgetViewExplorer(QWidget *parent)
	: QTreeWidget(parent)
	, _itemEditorOpened(nullptr)
{
	setRootIsDecorated(true);
	setAnimated(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);
	setHeaderLabels({"Object name", "Class"});
	setContextMenuPolicy(Qt::CustomContextMenu);
	invisibleRootItem()->setFlags(Qt::NoItemFlags);

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

QTreeWidgetItem *qtengine::TreeWidgetViewExplorer::createItemFor(libraryObjects::AObject *object, QTreeWidgetItem *parent, bool recursively, int index)
{
	if (!object) { return nullptr; }
	auto item = new QTreeWidgetItem({object->objectName(), object->className()});

	parent->insertChild(index == -1 || index > parent->childCount() ? parent->childCount() : index, item);
	_objects[item] = object;
	connect(object, &libraryObjects::AObject::propertyUpdated, this, &TreeWidgetViewExplorer::onPropertyUpdated);
	if (recursively)
		for (auto objectChild : object->children())
			createItemFor(objectChild, item, true, -1);
	return item;
}

QTreeWidgetItem *qtengine::TreeWidgetViewExplorer::createItemFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, bool recursively, int index)
{
	if (!object || !parent) { return nullptr; }
	return createItemFor(object, _objects.key(parent), recursively, index);
}

QTreeWidgetItem *qtengine::TreeWidgetViewExplorer::itemFor(libraryObjects::AObject *object) const
{
	return _objects.key(object);
}

void qtengine::TreeWidgetViewExplorer::expandRecursivelyItemFor(libraryObjects::AObject *object) const
{
	_objects.key(object)->setExpanded(true);
	for (auto objectChild : object->children())
		expandRecursivelyItemFor(objectChild);
}

void qtengine::TreeWidgetViewExplorer::removeItemFor(libraryObjects::AObject *object)
{
	if (!object) { return; }
	auto item = _objects.key(object);

	delete item;
	_objects.remove(item);
	disconnect(object, &libraryObjects::AObject::propertyUpdated, this, &TreeWidgetViewExplorer::onPropertyUpdated);
}

void qtengine::TreeWidgetViewExplorer::setCurrentItemFor(libraryObjects::AObject *object)
{
	if (!object) { return; }
	setCurrentItem(_objects.key(object));
}

void qtengine::TreeWidgetViewExplorer::dragMoveEvent(QDragMoveEvent *event)
{
	QTreeWidget::dragMoveEvent(event);

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

	auto mimeDataObject = dynamic_cast<const MimeDataObject *>(event->mimeData());
	if (!mimeDataObject) { return; }

	auto libraryObjectParent = libraryObjects::LibraryObjectManager::instance()->libraryObjectOf(_objects[item]->classHierarchy());
	if (!libraryObjectParent->libraryFunction()->functionDragFor(mimeDataObject->libraryObject()->classHierarchy()).isValid)
		event->ignore();
}

bool qtengine::TreeWidgetViewExplorer::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction)
{
	auto mimeDataObject = dynamic_cast<const MimeDataObject *>(data);
	if (!mimeDataObject) { return false; }

	emit libraryObjectDropped(_objects[parent], index, mimeDataObject->libraryObject(), mimeDataObject->reference());
	return false;
}

QMimeData *qtengine::TreeWidgetViewExplorer::mimeData(const QList<QTreeWidgetItem *> items) const
{
	auto object = _objects[items.front()];
	auto libraryObject = libraryObjects::LibraryObjectManager::instance()->libraryObjectOf(object->classHierarchy());

	QMimeData *mimeData = QTreeWidget::mimeData(items);
	auto mimeDataObject = new MimeDataObject(new libraryObjects::ObjectClass(object->object()->metaObject()), libraryObject, object);
	for (auto format : mimeData->formats())
		mimeDataObject->setData(format, mimeData->data(format));
	return mimeDataObject;
}

void qtengine::TreeWidgetViewExplorer::onPropertyUpdated(const QString &propertyName, const QVariant &propertyValue)
{
	if (propertyName == "objectName")
		_objects.key(dynamic_cast<libraryObjects::AObject*>(sender()))->setText(0, propertyValue.toString());
}
