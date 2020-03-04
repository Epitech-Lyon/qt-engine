/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ProjectManager
*/

#include "moc_ProjectManager.cpp"
#include "ProjectManager.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "MainWindow.hpp"
#include "DialogExportSettings.hpp"
#include "Exporter.hpp"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>
#include <QtCore/QDebug>

QJsonObject qtengine::ProjectManager::serialize() const
{
	QJsonArray jsonRecentProjects;
	for (auto recentFile : _recentsProject)
		jsonRecentProjects.append(recentFile);

	QJsonObject json;
	json["Current project"] = _projectPath;
	json["Recent projects"] = jsonRecentProjects;
	return json;
}

void qtengine::ProjectManager::deserialize(const QJsonObject &json)
{
	openProject(json["Current project"].toString());
	for (auto recentFileRef : json["Recent projects"].toArray())
		_recentsProject << recentFileRef.toString();
}

void qtengine::ProjectManager::openProject(const QString &projectPath)
{
	QFileInfo fileInfo(projectPath);
	if (projectPath.isEmpty() || !fileInfo.exists()) { return; }

	_projectDir = fileInfo.absolutePath();
	_projectPath = fileInfo.filePath();
	_projectName = fileInfo.baseName();
	_recentsProject.removeAll(_projectPath);
	_recentsProject.push_front(_projectPath);
	while (_recentsProject.size() > _maxRecentsProject)
		_recentsProject.pop_back();

	// Create View folder if not exist
	QDir dir(_projectDir + "/views");
	if (!dir.exists())
		dir.mkpath(".");

	emit projectChanged();
	emit projectDirChanged(_projectDir);
	emit projectPathChanged(_projectPath);
	emit projectNameChanged(_projectName);
	emit recentProjectsChanged(_recentsProject);
}

void qtengine::ProjectManager::onNewProject()
{
	QFileDialog dialog(Manager::instance()->mainWindow(), "New project", QDir::homePath(), "Project (*" + _projectExt + ")");
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::DontUseNativeDialog, true);
	dialog.setOption(QFileDialog::ShowDirsOnly, false);
	if (dialog.exec() != QDialog::Accepted) { return; }
	auto selectedDirectory = dialog.selectedFiles().front();

	// Create directory
	QDir dir(selectedDirectory);
	if (QFile(dir.path() + "/" + dir.dirName() + _projectExt).exists())
		if (QMessageBox::warning(Manager::instance()->mainWindow(), "New project", dir.path() + "/" + dir.dirName() + _projectExt + " already exists, Do you want to replace it ?", QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
			return;
	QFile::remove(dir.path() + "/" + dir.dirName() + _projectExt);
	dir.mkpath(".");

	// Create projectFile
	QFile file(dir.path() + "/" + dir.dirName() + _projectExt);
	if (!file.open(QIODevice::WriteOnly)) {
		qWarning() << "Can't create project path at" << dir.path() + "/" + dir.dirName() + _projectExt;
		return;
	}
	openProject(file.fileName());
}

void qtengine::ProjectManager::onOpenProject()
{
	openProject(QFileDialog::getOpenFileName(Manager::instance()->mainWindow(), "Open project", QDir::homePath(), "Project (*" + _projectExt + ")"));
}

void qtengine::ProjectManager::onExportProject()
{
	DialogExportSettings dialog(_projectDir + "/build", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted) {
		QStringList views;
		auto viewsInfo = QDir(_projectDir + "/views/", "*" + Manager::instance()->viewManager()->viewExtension()).entryInfoList(QDir::Files);
		for (auto viewInfo : viewsInfo)
			views << viewInfo.absoluteFilePath();

		auto exporter = new libraryObjects::Exporter(dialog.outputPath(), dialog.generateMain(), views);

		if (dialog.displayProgress()) {
			auto progressDialog = new QProgressDialog(Manager::instance()->mainWindow());
			connect(exporter, &libraryObjects::Exporter::currentViewExportedChanged, [progressDialog, viewsInfo](int index) {
				progressDialog->setLabelText("Export " + viewsInfo[index].completeBaseName());
			});
			connect(exporter, &libraryObjects::Exporter::currentViewExportedChanged, progressDialog, &QProgressDialog::setValue);
			connect(exporter, &QThread::finished, progressDialog, &QWidget::close);
			progressDialog->setAttribute(Qt::WA_DeleteOnClose);
			progressDialog->setCancelButton(nullptr);
			progressDialog->setMaximum(views.size());
			progressDialog->show();
		}

		connect(exporter, &QThread::finished, exporter, &QObject::deleteLater);
		exporter->start();
	}
}
