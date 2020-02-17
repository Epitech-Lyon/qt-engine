/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObjectManager
*/

#include "LibraryObjectManager.hpp"

#include "EWidget.hpp"
#include "ELabel.hpp"
#include "ELCDNumber.hpp"
#include "ESplitter.hpp"
#include "EStackedWidget.hpp"
#include "EToolBox.hpp"
#include "EPlainTextEdit.hpp"
#include "EScrollArea.hpp"
#include "ETextEdit.hpp"

#include "EVBoxLayout.hpp"
#include "EHBoxLayout.hpp"

libraryObjects::LibraryObjectManager::LibraryObjectManager()
{
	// QObject
	registerObject<EWidget>();

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
