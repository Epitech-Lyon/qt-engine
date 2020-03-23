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
#include "DialogSettingsExport.hpp"
#include "Exporter.hpp"

#include "LibraryObjectManager.hpp"
#include "CustomObject.hpp"

#include "DialogSettingsCreateView.hpp"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QProgressDialog>
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>

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
	for (auto recentFileRef : json["Recent projects"].toArray())
		if (QFileInfo::exists(recentFileRef.toString()))
			_recentsProject << recentFileRef.toString();
	openProject(json["Current project"].toString());
}

void qtengine::ProjectManager::openProject(const QString &projectPath)
{
	QFileInfo fileInfo(projectPath);
	if (projectPath.isEmpty() || !fileInfo.exists()) { return; }

	onCloseProject();

	_projectIsOpened = true;
	_projectPath = fileInfo.absoluteFilePath();
	_projectName = fileInfo.baseName();
	_recentsProject.removeAll(_projectPath);
	_recentsProject.push_front(_projectPath);
	while (_recentsProject.size() > _maxRecentsProject)
		_recentsProject.pop_back();

	QFile file(projectPath);
	QJsonObject json;
	if (file.open(QIODevice::ReadOnly)) {
		json = QJsonDocument::fromJson(file.readAll()).object();
		file.close();
	}

	for (auto viewPathRef : json["Views"].toArray()) {
		QFileInfo fileInfo(viewPathRef.toString());
		if (!fileInfo.exists()) { continue; }

		_views.append(fileInfo.absoluteFilePath());
		libraryObjects::CustomObject::registerAsLibraryObject(fileInfo.absoluteFilePath());
	}
	_lastOutputPath = json["outputPath"].toString();
	_lastGenerateMainFrom = json["generateMainFrom"].toString();
	_lastDisplayProgress = json["displayProgress"].toBool(true);

	emit projectOpened(_projectIsOpened);
	emit projectPathChanged(_projectPath);
	emit projectNameChanged(_projectName);
	emit recentProjectsChanged(_recentsProject);
	emitViewsChanged();

	Manager::instance()->mainWindow()->setTitleProject(_projectName);
}

void qtengine::ProjectManager::onNewProject()
{
	auto projectPath = QFileDialog::getSaveFileName(Manager::instance()->mainWindow(), "New project", QDir::homePath(), "Project (*" + _projectExt + ")");
	if (projectPath.isEmpty()) { return; }

	// Create directory
	QFileInfo fileInfo(projectPath.endsWith(_projectExt) ? projectPath : projectPath + _projectExt);
	if (fileInfo.exists())
		QFile::remove(fileInfo.absoluteFilePath());

	// Create projectFile
	QFile file(fileInfo.absoluteFilePath());
	if (!file.open(QIODevice::WriteOnly)) {
		qWarning() << "Can't create project at" << fileInfo.absoluteFilePath();
		return;
	}
	file.close();

	openProject(fileInfo.absoluteFilePath());
}

void qtengine::ProjectManager::onOpenProject()
{
	openProject(QFileDialog::getOpenFileName(Manager::instance()->mainWindow(), "Open project", QDir::homePath(), "Project (*" + _projectExt + ")"));
}

void qtengine::ProjectManager::onSaveProject()
{
	QJsonArray jsonViews;
	for (auto view : _views)
		jsonViews.append(view);

	QJsonObject json;
	json["Views"] = jsonViews;
	json["outputPath"] = _lastOutputPath;
	json["generateMainFrom"] = _lastGenerateMainFrom;
	json["displayProgress"] = _lastDisplayProgress;

	QFile file(_projectPath);
	if (file.open(QIODevice::WriteOnly)) {
		file.write(QJsonDocument(json).toJson());
		file.close();
	}
}

void qtengine::ProjectManager::onExportProject()
{
	QString lastOutputPath = QFileInfo::exists(_lastOutputPath) ? _lastOutputPath : QFileInfo(_projectPath).absolutePath() + "/generated";
	DialogSettingsExport dialog(lastOutputPath, _lastGenerateMainFrom, _lastDisplayProgress, Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted) {
		_lastOutputPath = dialog.outputPath();
		_lastGenerateMainFrom = dialog.generateMainFrom();
		_lastDisplayProgress = dialog.displayProgress();

		onSaveProject();

		auto exporter = new libraryObjects::Exporter(_lastOutputPath, _lastGenerateMainFrom, _views);

		Manager::instance()->viewManager()->onSaveView();
		if (_lastDisplayProgress) {
			auto progressDialog = new QProgressDialog(Manager::instance()->mainWindow());
			connect(exporter, &libraryObjects::Exporter::currentViewExportedChanged, [this, progressDialog](int index) {
				progressDialog->setLabelText("Export " + QFileInfo(_views[index]).completeBaseName());
			});
			connect(exporter, &libraryObjects::Exporter::currentViewExportedChanged, progressDialog, &QProgressDialog::setValue);
			connect(exporter, &libraryObjects::Exporter::error, progressDialog, &QWidget::close);
			connect(exporter, &libraryObjects::Exporter::error, Manager::instance()->mainWindow(), [](const QString &errorMessage) {
				QMessageBox::critical(Manager::instance()->mainWindow(), "Export", errorMessage);
			}, Qt::QueuedConnection);
			connect(exporter, &QThread::finished, progressDialog, &QWidget::close);
			progressDialog->setWindowFlags(Qt::Window);
			progressDialog->setAttribute(Qt::WA_DeleteOnClose);
			progressDialog->setCancelButton(nullptr);
			progressDialog->setMaximum(_views.size());
			progressDialog->setMinimumSize(progressDialog->sizeHint());
			progressDialog->show();
		}

		connect(exporter, &QThread::finished, exporter, &QObject::deleteLater);
		exporter->start();
	}
}

void qtengine::ProjectManager::onCloseProject()
{
	onSaveProject();

	Manager::instance()->viewManager()->closeView();
	_projectIsOpened = false;
	_projectPath.clear();
	_projectName.clear();
	_views.clear();
	libraryObjects::LibraryObjectManager::instance()->unregisterAllCustomObjects();
	emit projectOpened(_projectIsOpened);
	emit projectPathChanged(_projectPath);
	emit projectPathChanged(_projectName);
	emitViewsChanged();

	Manager::instance()->mainWindow()->setTitleProject("");
}

void qtengine::ProjectManager::onCreateView()
{
	QString viewPath = QFileInfo(_projectPath).absolutePath() + "/untitled";
	DialogSettingsCreateView dialog(viewPath + Manager::instance()->viewManager()->viewExtension(), Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted) {
		viewPath = QFileInfo(dialog.viewPath()).absoluteFilePath();

		if (dialog.isCopy())
			Manager::instance()->viewManager()->createViewFrom(viewPath, dialog.copyFromView());
		else
			Manager::instance()->viewManager()->createView(viewPath, dialog.libraryObject());
		libraryObjects::CustomObject::registerAsLibraryObject(viewPath);
		_views.append(viewPath);
		emitViewsChanged();
	}
}

void qtengine::ProjectManager::onImportView()
{
	QString viewPath = QFileDialog::getOpenFileName(Manager::instance()->mainWindow(), "Import view", QDir::homePath(), "View (*" + Manager::instance()->viewManager()->viewExtension() + ")");
	QFileInfo fileInfo(viewPath);
	if (!fileInfo.exists()) { return; }

	viewPath = fileInfo.absoluteFilePath();
	libraryObjects::CustomObject::registerAsLibraryObject(viewPath);
	_views.append(viewPath);
	emitViewsChanged();
}

void qtengine::ProjectManager::onRemoveView(const QString &viewPath)
{
	QFileInfo fileInfo(viewPath);
	if (!fileInfo.exists() || "." + fileInfo.completeSuffix() != Manager::instance()->viewManager()->viewExtension()) { return; }

	libraryObjects::LibraryObjectManager::instance()->unregisterCustomObject(fileInfo.fileName());
	_views.removeAll(fileInfo.absoluteFilePath());
	emitViewsChanged();
}

void qtengine::ProjectManager::emitViewsChanged()
{
	_views.removeDuplicates();
	_views.sort();
	emit viewsChanged(_views);
}
