/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelLibraryObject
*/

#include "ContentPanelLibraryObject.hpp"

#include "LibraryObjectManager.hpp"
#include "LibraryObjectMimeData.hpp"

qtengine::TreeLibraryObject::TreeLibraryObject(QWidget *parent)
	: QTreeWidget(parent)
{
	setRootIsDecorated(true);
	setAnimated(true);
	setDragDropMode(QAbstractItemView::DragOnly);
	setHeaderHidden(true);
}

void qtengine::TreeLibraryObject::addLibraryObject(libraryObjects::LibraryObject *libraryObject)
{
	auto item = addItem(invisibleRootItem(), libraryObject->classHierarchy());

	item->setIcon(0, libraryObject->icon());
	item->setFlags(item->flags() | Qt::ItemIsDragEnabled);
	_libraryObjects[item] = libraryObject;
}

QTreeWidgetItem *qtengine::TreeLibraryObject::addItem(QTreeWidgetItem *parent, const QString &classHierarchy)
{
	QTreeWidgetItem *child = nullptr;
	QStringList newClassHierarchy = classHierarchy.split("::");
	QString name = newClassHierarchy.takeFirst();

	for (int i = 0; i < parent->childCount(); i += 1)
		if (parent->child(i)->text(0) == name)
			child = parent->child(i);
	if (!child) {
		child = new QTreeWidgetItem(parent, {name});
		child->setFlags(child->flags() ^ Qt::ItemIsDragEnabled);
	}
	return newClassHierarchy.size() == 0 ? child : addItem(child, newClassHierarchy.join("::"));
}

QMimeData *qtengine::TreeLibraryObject::mimeData(const QList<QTreeWidgetItem *> items) const
{
	QMimeData *mimeData = QTreeWidget::mimeData(items);
	auto libraryObjectMimeData = new libraryObjects::LibraryObjectMimeData(_libraryObjects[items.front()]);

	for (auto format : mimeData->formats())
		libraryObjectMimeData->setData(format, mimeData->data(format));
	return libraryObjectMimeData;
}

qtengine::ContentPanelLibraryObject::ContentPanelLibraryObject(QWidget *parent)
	: ContentPanelBase("Library Object", parent)
{
}

void qtengine::ContentPanelLibraryObject::init()
{
	_tree = new TreeLibraryObject(this);
	_mainLayout->addWidget(_tree);

	for (auto libraryObject : libraryObjects::LibraryObjectManager::instance()->libraryObjects())
		_tree->addLibraryObject(libraryObject);
	_tree->expandAll();
}
