/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewManager
*/

#include "moc_ViewManager.cpp"
#include "ViewManager.hpp"

#include "ViewManagement.hpp"

#include "Manager.hpp"
#include "MainWindow.hpp"

#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtWidgets/QInputDialog>

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
	if (viewPath.isEmpty() || !fileInfo.exists() || fileInfo.completeSuffix() != _viewExt) { return; }

	_viewPath = fileInfo.filePath();
	_viewName = fileInfo.baseName();
	QFile file(_viewPath);
	if (file.open(QIODevice::ReadOnly)) {
		_viewObject = QJsonDocument::fromJson(file.readAll()).object();
		file.close();
	}

	emit viewPathChanged(_viewPath);
	emit viewNameChanged(_viewName);
	emit viewObjectChanged(_viewObject);
}

void qtengine::ViewManager::onCreateView(const QString &viewPath)
{
	QFileInfo fileInfo(viewPath);
	if (viewPath.isEmpty() || !fileInfo.exists() || fileInfo.completeSuffix() != _viewExt) { return; }

	auto widget = new QWidget;
	QJsonObject json = ViewManagement(widget).serialize();
	delete widget;

	QFile file(viewPath);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(QJsonDocument(json).toJson());
		file.close();
	}
	openView(viewPath);
}

void qtengine::ViewManager::onSaveView(const QJsonObject &viewObject)
{
	QFileInfo fileInfo(_viewPath);
	if (_viewPath.isEmpty() || !fileInfo.exists() || fileInfo.completeSuffix() != _viewExt) { return; }

	QFile file(_viewPath);
	if (file.open(QIODevice::WriteOnly)) {
		_viewObject = viewObject;
		file.write(QJsonDocument(_viewObject).toJson());
		file.close();
	}
}

void qtengine::ViewManager::onSaveViewAs(const QJsonObject &viewObject)
{
	QFileInfo fileInfo(_viewPath);
	if (_viewPath.isEmpty() || !fileInfo.exists() || fileInfo.completeSuffix() != _viewExt) { return; }

	bool ok = false;
	auto viewPath = QInputDialog::getText(Manager::instance()->mainWindow(), "Save view as", "Choose a new name", QLineEdit::Normal, _viewPath, &ok);
	if (!ok || viewPath.isEmpty()) {
		viewPath = viewPath.endsWith(_viewExt) ? viewPath : viewPath + _viewExt;
		QFile file(viewPath);
		if (file.open(QIODevice::WriteOnly)) {
			file.write(QJsonDocument(viewObject).toJson());
			file.close();
		}
		openView(viewPath);
	}
}
