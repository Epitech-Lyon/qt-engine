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

#include "Object.hpp"
#include "ObjectClass.hpp"

qtengine::ViewManager::ViewManager()
	: _viewObject(nullptr)
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
	openView(json["Current view"].toString());
}

void qtengine::ViewManager::closeView()
{
	auto oldViewObject = _viewObject;
	auto oldViewObjectClass = _viewObjectClass;

	_viewPath.clear();
	_viewName.clear();
	_viewObject = nullptr;
	_viewObjectClass = nullptr;
	emit viewPathChanged("");
	emit viewNameChanged("");
	emit viewObjectChanged(nullptr);
	emit viewObjectClassChanged(nullptr);
	setCurrentObject(nullptr);

	delete oldViewObject;
	delete oldViewObjectClass;
}

void qtengine::ViewManager::openView(const QString &viewPath)
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
	_viewObject = libraryObjects::ViewConverter().deserialize(json["Engine"].toObject());
	if (!_viewObject) { qCritical() << "Impossible to open" << _viewPath; return; }
	_viewPath = fileInfo.absoluteFilePath();
	_viewName = fileInfo.baseName();
	_viewObjectClass = new libraryObjects::ObjectClass();
	_viewObjectClass->deserialize(json["Class"].toObject());

	emit viewPathChanged(_viewPath);
	emit viewNameChanged(_viewName);
	emit viewObjectChanged(_viewObject);
	emit viewObjectClassChanged(_viewObjectClass);
	setCurrentObject(_viewObject);
}

void qtengine::ViewManager::onCreateView(const QString &viewPath)
{
	QFileInfo fileInfo(viewPath);
	if (!fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return; }

	closeView();

	_viewPath = fileInfo.filePath();
	_viewName = fileInfo.baseName();
	_viewObject = nullptr;

	emit viewPathChanged(_viewPath);
	emit viewNameChanged(_viewName);
	emit viewObjectChanged(nullptr);
	setCurrentObject(nullptr);
}

bool qtengine::ViewManager::createView(libraryObjects::AObject *viewObject)
{
	QFileInfo fileInfo(_viewPath);
	if (!fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return false; }

	_viewObject = viewObject;
	emit viewObjectChanged(_viewObject);
	setCurrentObject(_viewObject);
	onSaveView();
	return true;
}

void qtengine::ViewManager::onSaveView()
{
	QFileInfo fileInfo(_viewPath);
	if (!fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return; }

	QJsonObject json;
	json["Engine"] = libraryObjects::ViewConverter().serialize(_viewObject);
	json["Class"] = _viewObjectClass->serialize();

	QFile file(_viewPath);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(QJsonDocument(json).toJson());
		file.close();
	}
}

void qtengine::ViewManager::onSaveViewAs()
{
	QFileInfo fileInfo(_viewPath);
	if (!fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return; }

	bool ok = false;
	auto viewPath = QInputDialog::getText(Manager::instance()->mainWindow(), "Save view as", "Choose a new name", QLineEdit::Normal, _viewPath, &ok);
	if (ok && !viewPath.isEmpty()) {
		_viewPath = viewPath.endsWith(_viewExt) ? viewPath : viewPath + _viewExt;
		_viewName = QFileInfo(_viewPath).baseName();
		onSaveView();
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
