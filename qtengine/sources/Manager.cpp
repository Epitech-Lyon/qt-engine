/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Manager
*/

#include "moc_Manager.cpp"
#include "Manager.hpp"

#include "MainWindow.hpp"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

qtengine::Manager::Manager()
	: _mainWindow(new MainWindow(this))
{
	_mainWindow->showMaximized();
}

qtengine::Manager::~Manager()
{
}

qtengine::Manager *qtengine::Manager::instance()
{
	static Manager manager;

	return &manager;
}

void qtengine::Manager::onNewProject()
{
	QFileDialog dialog(_mainWindow, "New project", QDir::homePath(), "Project (*" + _projectExt + ")");
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::DontUseNativeDialog, true);
	dialog.setOption(QFileDialog::ShowDirsOnly, false);
	if (dialog.exec() != QDialog::Accepted) { return; }
	auto selectedDirectory = dialog.selectedFiles().front();

	// Create directory
	QDir dir(selectedDirectory);
	if (QFile(dir.path() + "/" + dir.dirName() + _projectExt).exists())
		if (QMessageBox::warning(_mainWindow, "New project", dir.path() + "/" + dir.dirName() + _projectExt + " already exists, Do you want to replace it ?", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
			return;
	dir.removeRecursively();
	dir.mkpath(".");

	// Create projectFile
	QFile file(dir.path() + "/" + dir.dirName() + _projectExt);
	if (!file.open(QIODevice::WriteOnly)) {
		qWarning() << "Can't create project path at" << dir.path() + "/" + dir.dirName() + _projectExt;
		return;
	}
	openProject(file.fileName());
}

void qtengine::Manager::onOpenProject()
{
	auto selectedProject = QFileDialog::getOpenFileName(_mainWindow, "Open project", QDir::homePath(), "Project (*" + _projectExt + ")");
	if (!QFile(selectedProject).exists()) { return; }

	openProject(selectedProject);
}

void qtengine::Manager::onSaveProject()
{
}

void qtengine::Manager::onSaveProjectAs()
{
}

void qtengine::Manager::openProject(const QString &projectPath)
{
	_projectPath = projectPath;
	emit projectChanged();
	emit projectDirChanged(_projectPath);
	emit projectPathChanged(_projectPath);
	emit projectNameChanged(_projectPath);
}
