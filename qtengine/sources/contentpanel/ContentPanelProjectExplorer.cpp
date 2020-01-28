/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelProjectExplorer
*/

#include "ContentPanelProjectExplorer.hpp"

#include "Manager.hpp"
#include "ProjectManager.hpp"
#include "ViewManager.hpp"

#include <QtWidgets/QToolBar>

qtengine::ContentPanelProjectExplorer::ContentPanelProjectExplorer(QWidget *parent)
	: ContentPanelBase("Project Explorer", parent)
{
}

void qtengine::ContentPanelProjectExplorer::init()
{
	_treeView = new QTreeView(this);
	_fsModel = new QFileSystemModel(_treeView);
	auto onProjectDirChanged = [this](const QString &projectDir) {
		if (projectDir.isEmpty())
			_treeView->setModel(nullptr);
		else {
			_treeView->setModel(_fsModel);
			_treeView->setRootIndex(_fsModel->setRootPath(projectDir + "/views"));
		}
	};

	_fsModel->setNameFilters({"*.view"});
	_fsModel->setReadOnly(false);
	_treeView->setModel(_fsModel);
	_treeView->setHeaderHidden(true);
	_treeView->setDropIndicatorShown(true);
	_treeView->setDragDropMode(QAbstractItemView::InternalMove);
	_treeView->setEditTriggers(QTreeView::NoEditTriggers);
	for (int i = 1; i < _fsModel->columnCount(); ++i)
		_treeView->hideColumn(i);
	_mainLayout->addWidget(_treeView);

	onProjectDirChanged(Manager::instance()->projectManager()->projectDir());
	connect(Manager::instance()->projectManager(), &ProjectManager::projectDirChanged, onProjectDirChanged);

	_treeView->setCurrentIndex(_fsModel->index(Manager::instance()->viewManager()->currentView()));
	connect(_treeView, &QTreeView::clicked, this, &ContentPanelProjectExplorer::onModelIndexClicked);

	ContentPanelBase::init();
}

QToolBar *qtengine::ContentPanelProjectExplorer::initToolBar()
{
	auto toolbar = new QToolBar(this);

	toolbar->addAction(QIcon(":icon_collapse"), "", _treeView, &QTreeView::collapseAll);
	toolbar->addAction(QIcon(":icon_expand"), "", _treeView, &QTreeView::expandAll);
	toolbar->addSeparator();
	toolbar->addAction(QIcon(":icon_create"), "", this, &ContentPanelProjectExplorer::onCreateFile);
	toolbar->addAction(QIcon(":icon_rename"), "", this, &ContentPanelProjectExplorer::onRenameFile);
	toolbar->addAction(QIcon(":icon_delete"), "", this, &ContentPanelProjectExplorer::onDeleteFile);
	return toolbar;
}

void qtengine::ContentPanelProjectExplorer::onModelIndexClicked(const QModelIndex &modelIndex)
{
	Manager::instance()->viewManager()->openView(_fsModel->filePath(modelIndex));
}

void qtengine::ContentPanelProjectExplorer::onCreateFile()
{
	// Ask for create file based on QWidget/QDialog
}

void qtengine::ContentPanelProjectExplorer::onRenameFile()
{
	auto index = _treeView->currentIndex();

	if (index.isValid())
		_treeView->edit(index);
}

void qtengine::ContentPanelProjectExplorer::onDeleteFile()
{
	auto index = _treeView->currentIndex();

	if (index.isValid())
		_fsModel->remove(index);
}
