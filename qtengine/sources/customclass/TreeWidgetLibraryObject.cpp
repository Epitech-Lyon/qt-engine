/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TreeWidgetLibraryObject
*/

#include "TreeWidgetLibraryObject.hpp"

#include "LibraryObject.hpp"
#include "MimeDataObject.hpp"
#include "ObjectClass.hpp"

qtengine::TreeWidgetLibraryObject::TreeWidgetLibraryObject(QWidget *parent)
	: QTreeWidget(parent)
{
	setRootIsDecorated(true);
	setAnimated(true);
	setDragDropMode(QAbstractItemView::DragOnly);
	setHeaderHidden(true);
}

void qtengine::TreeWidgetLibraryObject::addLibraryObject(libraryObjects::LibraryObject *libraryObject)
{
	auto item = addItem(invisibleRootItem(), libraryObject->classHierarchy());

	item->setIcon(0, libraryObject->icon());
	item->setFlags(item->flags() | Qt::ItemIsDragEnabled);
	_libraryObjects[item] = libraryObject;
}

QTreeWidgetItem *qtengine::TreeWidgetLibraryObject::addItem(QTreeWidgetItem *parent, const QString &classHierarchy)
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

QMimeData *qtengine::TreeWidgetLibraryObject::mimeData(const QList<QTreeWidgetItem *> items) const
{
	QMimeData *mimeData = QTreeWidget::mimeData(items);
	auto mimeDataObject = new MimeDataObject(this, new libraryObjects::ObjectClass(), _libraryObjects[items.front()]);

	for (auto format : mimeData->formats())
		mimeDataObject->setData(format, mimeData->data(format));
	return mimeDataObject;
}
