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

#include "Object.hpp"

qtengine::ViewManager::ViewManager()
	: _viewObject(nullptr)
	, _currentObject(nullptr)
{
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

void qtengine::ViewManager::openView(const QString &viewPath)
{
	QFileInfo fileInfo(viewPath);
	if (viewPath.isEmpty() || !fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return; }

	_viewPath = fileInfo.filePath();
	_viewName = fileInfo.baseName();

	QJsonObject json;
	QFile file(_viewPath);
	if (file.open(QIODevice::ReadOnly)) {
		json = QJsonDocument::fromJson(file.readAll()).object();
		file.close();
	}
	_viewObject = libraryObjects::ViewConverter::deserialize(json);

	emit viewPathChanged(_viewPath);
	emit viewNameChanged(_viewName);
	emit viewObjectChanged(_viewObject);
}

void qtengine::ViewManager::onCreateView(const QString &viewPath)
{
	QFileInfo fileInfo(viewPath);
	if (viewPath.isEmpty() || !fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return; }

	openView(viewPath);
}

void qtengine::ViewManager::onSaveView()
{
	QFileInfo fileInfo(_viewPath);
	if (_viewPath.isEmpty() || !fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return; }

	QJsonObject json = libraryObjects::ViewConverter::serialize(_viewObject);
	QFile file(_viewPath);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(QJsonDocument(json).toJson());
		file.close();
	}
}

void qtengine::ViewManager::onSaveViewAs()
{
	QFileInfo fileInfo(_viewPath);
	if (_viewPath.isEmpty() || !fileInfo.exists() || "." + fileInfo.completeSuffix() != _viewExt) { return; }

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
