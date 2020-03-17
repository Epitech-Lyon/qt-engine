/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TreeWidgetProjectExplorer
*/

#include "moc_TreeWidgetProjectExplorer.cpp"
#include "TreeWidgetProjectExplorer.hpp"

#include "ViewManager.hpp"

#include "LibraryObjectManager.hpp"
#include "LibraryObject.hpp"
#include "MimeDataObject.hpp"
#include "ObjectClass.hpp"
#include "types/includes/Constructor.hpp"

#include <QtCore/QFileInfo>

qtengine::TreeWidgetProjectExplorer::TreeWidgetProjectExplorer(QWidget *parent)
	: QTreeWidget(parent)
{
	setRootIsDecorated(true);
	setAnimated(true);
	setDragDropMode(QAbstractItemView::DragOnly);
	setDropIndicatorShown(true);
	setHeaderHidden(true);

	connect(this, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem *item, int) { emit viewDoubleClicked(_viewPath[item]); });
}

QTreeWidgetItem *qtengine::TreeWidgetProjectExplorer::addView(const QString &viewPath)
{
	QFileInfo fileInfo(viewPath);
	auto item = new QTreeWidgetItem(invisibleRootItem(), { fileInfo.baseName() });

	_viewPath[item] = fileInfo.absoluteFilePath();
	return item;
}

void qtengine::TreeWidgetProjectExplorer::removeView(const QString &viewPath)
{
	auto item = _viewPath.key(viewPath);

	_viewPath.remove(item);
	delete item;
}

void qtengine::TreeWidgetProjectExplorer::clearViews()
{
	clear();
	_viewPath.clear();
}

void qtengine::TreeWidgetProjectExplorer::setCurrentView(const QString &viewPath)
{
	setCurrentItem(_viewPath.key(viewPath));
}

QString qtengine::TreeWidgetProjectExplorer::currentView() const
{
	return _viewPath[currentItem()];
}

QMimeData *qtengine::TreeWidgetProjectExplorer::mimeData(const QList<QTreeWidgetItem *> items) const
{
	QFileInfo fileInfo(_viewPath[items.front()]);
	auto libraryObject = libraryObjects::LibraryObjectManager::instance()->customObject(fileInfo.fileName());

	QMimeData *mimeData = QTreeWidget::mimeData(items);
	auto mimeDataObject = new MimeDataObject(ViewManager::getViewObjectClass(fileInfo.absoluteFilePath()), libraryObject);
	for (auto format : mimeData->formats())
		mimeDataObject->setData(format, mimeData->data(format));
	return mimeDataObject;
}
