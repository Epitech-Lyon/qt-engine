/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Manager
*/

#include "Manager.hpp"

#include "MainWindow.hpp"
#include "ProjectManager.hpp"
#include "LayoutManager.hpp"

#include "BoxTheme.hpp"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtWidgets/QApplication>
#include <QtCore/QFile>

qtengine::Manager::Manager()
	: _projectManager(new ProjectManager)
	, _layoutManager(new LayoutManager)
	, _mainWindow(new MainWindow(this))
{
}

qtengine::Manager::~Manager()
{
	delete _projectManager;
	delete _layoutManager;
}

qtengine::Manager *qtengine::Manager::instance()
{
	static Manager manager;

	return &manager;
}

void qtengine::Manager::init()
{
	QFile file(qApp->applicationDirPath() + "/settings.ini");
	QJsonObject json;
	if (file.open(QIODevice::ReadOnly)) {
		json = QJsonDocument::fromJson(file.readAll()).object();
		file.close();
	}
	_theme = json["Theme"].toString();
	BoxTheme().applyTheme(_theme);

	_projectManager->deserialize(json["Project manager"].toObject());
	_projectManager->openProject(json["Current project"].toString());
	_layoutManager->deserialize(json["Layout manager"].toObject());
	_layoutManager->openLayout(json["Current layout"].toString());
}

void qtengine::Manager::save()
{
	QJsonObject json;
	json["Theme"] = _theme;
	json["Project manager"] = _projectManager->serialize();
	json["Current project"] = _projectManager->projectPath();
	json["Layout manager"] = _layoutManager->serialize();
	json["Current layout"] = _layoutManager->layoutName();

	QFile file(qApp->applicationDirPath() + "/settings.ini");
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		file.write(QJsonDocument(json).toJson());
		file.close();
	}
}

void qtengine::Manager::onTheme()
{
	BoxTheme boxTheme(_mainWindow);
	
	if (boxTheme.exec() == QDialog::Accepted)
		_theme = boxTheme.theme();
}
