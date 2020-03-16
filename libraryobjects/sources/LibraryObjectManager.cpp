/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObjectManager
*/

#include "LibraryObjectManager.hpp"
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include "EFileSystemModel.hpp"

#include "EWidget.hpp"
#include "ELabel.hpp"
#include "ELCDNumber.hpp"
#include "ESplitter.hpp"
#include "EStackedWidget.hpp"
#include "EToolBox.hpp"
#include "EPlainTextEdit.hpp"
#include "EScrollArea.hpp"
#include "ETextEdit.hpp"
#include "EListView.hpp"
#include "ETreeView.hpp"
#include "ETableView.hpp"

#include "EVBoxLayout.hpp"
#include "EHBoxLayout.hpp"

libraryObjects::LibraryObjectManager::LibraryObjectManager()
{
	// QObject
	registerObject<EWidget>();

	// QObject::QAbstractItemModel
	registerObject<EFileSystemModel>();

	// QObject::QWidget::QFrame
	registerObject<ELabel>();
	registerObject<ELCDNumber>();
	registerObject<ESplitter>();
	registerObject<EStackedWidget>();
	registerObject<EToolBox>();

	// QObject::QWidget::QFrame::QAbstractScrollArea
	registerObject<EPlainTextEdit>();
	registerObject<ETextEdit>();
	registerObject<EScrollArea>();

	// QObject::QWidget::QFrame::QAbstractScrollArea::QAbstractItemView::QListView
	registerObject<EListView>();

	// QObject::QWidget::QFrame::QAbstractScrollArea::QAbstractItemView::QTreeView
	registerObject<ETreeView>();

	// QObject::QWidget::QFrame::QAbstractScrollArea::QAbstractItemView::QTableView
	registerObject<ETableView>();

	// QObject::QLayout::QBoxLayout
	registerObject<EVBoxLayout>();
	registerObject<EHBoxLayout>();
}

libraryObjects::LibraryObjectManager::~LibraryObjectManager()
{
	for (auto libraryObject : libraryObjects())
		delete libraryObject;
}

libraryObjects::LibraryObjectManager *libraryObjects::LibraryObjectManager::instance()
{
	static LibraryObjectManager libraryObjectManager;

	return &libraryObjectManager;
}

QList<libraryObjects::LibraryObject *> libraryObjects::LibraryObjectManager::libraryObjects() const
{
	auto libraryObjects = _libraryObjects;

	for (auto customObject : _customObjects)
		libraryObjects.removeAll(customObject);
	return libraryObjects;
}

libraryObjects::LibraryObject *libraryObjects::LibraryObjectManager::libraryObjectOf(const QString &classHierarchy) const
{
	for (auto libraryObject : _libraryObjects)
		if (libraryObject->classHierarchy() == classHierarchy)
			return libraryObject;
	return nullptr;
}

libraryObjects::LibraryObject *libraryObjects::LibraryObjectManager::libraryObjectOfClassName(const QString &className) const
{
	for (auto libraryObject : _libraryObjects)
		if (libraryObject->className() == className)
			return libraryObject;
	return nullptr;
}

void libraryObjects::LibraryObjectManager::registerCustomObject(const QString &name, LibraryObject *libraryObject)
{
	_customObjects[name] = libraryObject;
	_libraryObjects.append(libraryObject);
}

void libraryObjects::LibraryObjectManager::unregisterCustomObject(const QString &name)
{
	auto libraryObject = _customObjects.take(name);

	_libraryObjects.removeAll(libraryObject);
	delete libraryObject;
}

void libraryObjects::LibraryObjectManager::unregisterAllCustomObjects()
{
	for (auto libraryObject : _customObjects) {
		_libraryObjects.removeAll(libraryObject);
		delete libraryObject;
	}
	_customObjects.clear();
}
