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

#include "EObject.hpp"
#include "ETimer.hpp"
#include "EAction.hpp"
#include "EWidget.hpp"
#include "ELineEdit.hpp"
#include "EProgressBar.hpp"
#include "EMenu.hpp"
#include "EMenuBar.hpp"
#include "EComboBox.hpp"
#include "EFontComboBox.hpp"
#include "ECheckBox.hpp"
#include "EPushButton.hpp"
#include "ERadioButton.hpp"
#include "EToolButton.hpp"
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
	registerObject<EObject>();
	registerObject<ETimer>();
	registerObject<EAction>();

	// QObject::QAbstractItemModel
	registerObject<EFileSystemModel>();

	// QObject::QWidget
	registerObject<EWidget>();
	registerObject<ELineEdit>();
	registerObject<EProgressBar>();
	registerObject<EMenu>();
	registerObject<EMenuBar>();

	// QObject::QWidget::QComboBox
	registerObject<EComboBox>();
	registerObject<EFontComboBox>();

	// QObject::QWidget::QAbstractButton
	registerObject<ECheckBox>();
	registerObject<EPushButton>();
	registerObject<ERadioButton>();
	registerObject<EToolButton>();

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

bool libraryObjects::LibraryObjectManager::isSubClassOf(const QString &classHierarchy, const QString &parentClassHierarchy)
{
	return classHierarchy.startsWith(parentClassHierarchy);
}

bool libraryObjects::LibraryObjectManager::isSubClassOf(const AObject *object, const AObject *objectParent)
{
	if (!object || !objectParent) { return false; }

	return isSubClassOf(object->classHierarchy(), objectParent->classHierarchy());
}

bool libraryObjects::LibraryObjectManager::isSubClassOf(const LibraryObject *libraryObject, const LibraryObject *libraryObjectParent)
{
	if (!libraryObject || !libraryObjectParent) { return false; }

	return isSubClassOf(libraryObject->classHierarchy(), libraryObjectParent->classHierarchy());
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

libraryObjects::LibraryObject *libraryObjects::LibraryObjectManager::libraryObjectOfType(const QString &type) const
{
	if (!types::ClassTypeManager::instance()->isCustomType(type)) { return nullptr; }

	QString className(type);
	className.chop(1);
	return libraryObjectOfClassName(className);
}

void libraryObjects::LibraryObjectManager::registerCustomObject(const QString &name, LibraryObject *libraryObject, FunctionMetaObject functionMetaObject)
{
	_customObjects[name] = libraryObject;
	_libraryObjects.append(libraryObject);
	_metaObjects[libraryObject] = functionMetaObject;
	types::ClassTypeManager::instance()->registerType(libraryObject->className() + "*");
}

void libraryObjects::LibraryObjectManager::unregisterCustomObject(const QString &name)
{
	auto libraryObject = _customObjects.take(name);

	_libraryObjects.removeAll(libraryObject);
	_metaObjects.remove(libraryObject);
	types::ClassTypeManager::instance()->unregisterType(libraryObject->className() + "*");
	delete libraryObject;
}

void libraryObjects::LibraryObjectManager::unregisterAllCustomObjects()
{
	for (auto libraryObject : _customObjects) {
		_libraryObjects.removeAll(libraryObject);
		_metaObjects.remove(libraryObject);
		types::ClassTypeManager::instance()->unregisterType(libraryObject->className() + "*");
		delete libraryObject;
	}
	_customObjects.clear();
}

libraryObjects::LibraryObjectManager::FunctionMetaObject libraryObjects::LibraryObjectManager::metaObjectOf(const QString &classHierarchy) const
{
	auto libraryObject = libraryObjectOf(classHierarchy);
	if (!libraryObject) { return nullptr; }

	return _metaObjects[libraryObject];
}

libraryObjects::LibraryObjectManager::FunctionMetaObject libraryObjects::LibraryObjectManager::metaObjectOfType(const QString &type) const
{
	auto libraryObject = libraryObjectOfType(type);
	if (!libraryObject) { return nullptr; }

	return _metaObjects[libraryObject];
}
