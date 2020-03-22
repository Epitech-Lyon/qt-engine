/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewManager
*/

#include "moc_ViewManager.cpp"
#include "ViewManager.hpp"

#include "ViewConverter.hpp"

#include "Manager.hpp"
#include "MainWindow.hpp"

#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtWidgets/QInputDialog>
#include <QtCore/QDebug>

#include "LibraryObject.hpp"
#include "ObjectManager.hpp"
#include "Object.hpp"
#include "ObjectClass.hpp"
#include "types/includes/Constructor.hpp"

qtengine::ViewManager::ViewManager()
	: _viewIsOpened(false)
	, _viewObject(nullptr)
	, _viewObjectClass(nullptr)
	, _currentObject(nullptr)
{
}

qtengine::ViewManager::~ViewManager()
{
	delete _viewObject;
	delete _viewObjectClass;
}

QJsonObject qtengine::ViewManager::serialize() const
{
	QJsonObject json;
	json["Current view"] = _viewPath;
	return json;
}

void qtengine::ViewManager::deserialize(const QJsonObject &json)
{
	onOpenView(json["Current view"].toString());
}

void qtengine::ViewManager::createView(const QString &viewPath, libraryObjects::LibraryObject *libraryObject)
{
	QFileInfo fileInfo(viewPath);
	if ("." + fileInfo.completeSuffix() != _viewExt) { return; }

	closeView();

	QFile file(viewPath);
	file.open(QIODevice::WriteOnly);
	file.close();

	_viewIsOpened = true;
	_viewPath = fileInfo.filePath();
	_viewName = fileInfo.baseName();
	_viewObject = libraryObject->constructor();
	libraryObjects::ObjectManager::instance()->setObjectAsRoot(_viewObject);
	_viewObjectClass = new libraryObjects::ObjectClass();

	auto constructor = new types::Constructor();
	constructor->setClassName(_viewName);
	_viewObjectClass->addClassType(constructor);

	onSaveView();

	emit viewOpened(_viewIsOpened);
	emit viewPathChanged(_viewPath);
	emit viewNameChanged(_viewName);
	emit viewObjectChanged(_viewObject);
	emit viewObjectClassChanged(_viewObjectClass);
	setCurrentObject(_viewObject);

	Manager::instance()->mainWindow()->setTitleView(_viewName);
}

void qtengine::ViewManager::createViewFrom(const QString &viewPath, const QString &viewPathSource)
{
	QFileInfo fileInfoDest(viewPath);
	if ("." + fileInfoDest.completeSuffix() != _viewExt) { return; }

	QFileInfo fileInfoSrc(viewPathSource);
	if (!fileInfoSrc.exists() || "." + fileInfoSrc.completeSuffix() != _viewExt) { return; }

	QFile::copy(fileInfoSrc.absoluteFilePath(), fileInfoDest.absoluteFilePath());
	onOpenView(fileInfoDest.absoluteFilePath());
}

void qtengine::ViewManager::closeView()
{
	onSaveView();

	auto oldViewObject = _viewObject;
	auto oldViewObjectClass = _viewObjectClass;

	_viewIsOpened = false;
	_viewPath.clear();
	_viewName.clear();
	_viewObject = nullptr;
	_viewObjectClass = nullptr;
	emit viewOpened(false);
	emit viewPathChanged("");
	emit viewNameChanged("");
	emit viewObjectChanged(nullptr);
	emit viewObjectClassChanged(nullptr);
	setCurrentObject(nullptr);

	Manager::instance()->mainWindow()->setTitleView("");

	delete oldViewObject;
	delete oldViewObjectClass;
}

void qtengine::ViewManager::onOpenView(const QString &viewPath)
{
	QFileInfo fileInfo(viewPath);
	if (!fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return; }

	closeView();

	QJsonObject json;
	QFile file(fileInfo.absoluteFilePath());
	if (file.open(QIODevice::ReadOnly)) {
		json = QJsonDocument::fromJson(file.readAll()).object();
		file.close();
	}
	_viewIsOpened = true;
	_viewObject = libraryObjects::ViewConverter().deserialize(json["Engine"].toObject());
	libraryObjects::ObjectManager::instance()->setObjectAsRoot(_viewObject);
	if (!_viewObject) { qCritical() << "Impossible to open" << _viewPath; return; }
	_viewPath = fileInfo.absoluteFilePath();
	_viewName = fileInfo.baseName();
	_viewObjectClass = new libraryObjects::ObjectClass();
	_viewObjectClass->deserialize(json["Class"].toObject());

	emit viewOpened(_viewIsOpened);
	emit viewPathChanged(_viewPath);
	emit viewNameChanged(_viewName);
	emit viewObjectChanged(_viewObject);
	emit viewObjectClassChanged(_viewObjectClass);
	setCurrentObject(_viewObject);

	Manager::instance()->mainWindow()->setTitleView(_viewName);
}

void qtengine::ViewManager::onSaveView()
{
	QFileInfo fileInfo(_viewPath);
	if (!fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return; }

	emit saveRequested();

	QJsonObject json;
	json["Engine"] = libraryObjects::ViewConverter().serialize(_viewObject);
	json["Class"] = _viewObjectClass->serialize();

	QFile file(_viewPath);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(QJsonDocument(json).toJson());
		file.close();
	}
}

void qtengine::ViewManager::setCurrentObject(libraryObjects::AObject *currentObject)
{
	_currentObject = currentObject;
	emit currentObjectChanged(currentObject);
}

libraryObjects::AObject *qtengine::ViewManager::getViewObject(const QString &viewPath)
{
	QFileInfo fileInfo(viewPath);
	if (!fileInfo.exists() || "." + fileInfo.completeSuffix() != Manager::instance()->viewManager()->viewExtension()) { return nullptr; }

	QJsonObject json;
	QFile file(fileInfo.filePath());
	if (file.open(QIODevice::ReadOnly)) {
		json = QJsonDocument::fromJson(file.readAll()).object();
		file.close();
	}
	return libraryObjects::ViewConverter().deserialize(json["Engine"].toObject());
}

libraryObjects::ObjectClass *qtengine::ViewManager::getViewObjectClass(const QString &viewPath)
{
	QFileInfo fileInfo(viewPath);
	if (!fileInfo.exists() || "." + fileInfo.completeSuffix() != Manager::instance()->viewManager()->viewExtension()) { return nullptr; }

	QJsonObject json;
	QFile file(fileInfo.filePath());
	if (file.open(QIODevice::ReadOnly)) {
		json = QJsonDocument::fromJson(file.readAll()).object();
		file.close();
	}
	auto objectClass = new libraryObjects::ObjectClass();

	objectClass->deserialize(json["Class"].toObject());
	return objectClass;
}
