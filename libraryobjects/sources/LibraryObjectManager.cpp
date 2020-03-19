/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObjectManager
*/

#include "LibraryObjectManager.hpp"

#include "ClassTypeManager.hpp"

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

	///////////////////////////////////////////////////////////////////////////////

	// Register Type
	auto classTypeManager = types::ClassTypeManager::instance();
	classTypeManager->registerType(QMetaType::Void);
	classTypeManager->registerType(QMetaType::Int);
	classTypeManager->registerType(QMetaType::Double);
	classTypeManager->registerType(QMetaType::Bool);
	classTypeManager->registerType(QMetaType::QString);
	classTypeManager->registerType(QMetaType::QDate);
	classTypeManager->registerType(QMetaType::QTime);
	classTypeManager->registerType(QMetaType::QDateTime);
	classTypeManager->registerType(QMetaType::QKeySequence);
	classTypeManager->registerType(QMetaType::QChar);
	classTypeManager->registerType(QMetaType::QLocale);
	classTypeManager->registerType(QMetaType::QPoint);
	classTypeManager->registerType(QMetaType::QPointF);
	classTypeManager->registerType(QMetaType::QSize);
	classTypeManager->registerType(QMetaType::QSizeF);
	classTypeManager->registerType(QMetaType::QRect);
	classTypeManager->registerType(QMetaType::QRectF);
	classTypeManager->registerType(QMetaType::QColor);
	classTypeManager->registerType(QMetaType::QSizePolicy);
	classTypeManager->registerType(QMetaType::QFont);
	classTypeManager->registerType(QMetaType::QCursor);
	for (auto libraryObject : _libraryObjects)
		classTypeManager->registerType(libraryObject->className() + "*");
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
	types::ClassTypeManager::instance()->registerType(libraryObject->className() + "*");
}

void libraryObjects::LibraryObjectManager::unregisterCustomObject(const QString &name)
{
	auto libraryObject = _customObjects.take(name);

	_libraryObjects.removeAll(libraryObject);
	types::ClassTypeManager::instance()->unregisterType(libraryObject->className() + "*");
	delete libraryObject;
}

void libraryObjects::LibraryObjectManager::unregisterAllCustomObjects()
{
	for (auto libraryObject : _customObjects) {
		_libraryObjects.removeAll(libraryObject);
		types::ClassTypeManager::instance()->unregisterType(libraryObject->className() + "*");
		delete libraryObject;
	}
	_customObjects.clear();
}
