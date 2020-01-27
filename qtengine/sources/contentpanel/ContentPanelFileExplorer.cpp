/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelFileExplorer
*/

#include "ContentPanelFileExplorer.hpp"

#include "Manager.hpp"
#include "ProjectManager.hpp"

#include <QtWidgets/QToolBar>

qtengine::ContentPanelFileExplorer::ContentPanelFileExplorer(QWidget *parent)
	: ContentPanelBase("File Explorer", parent)
{
}

void qtengine::ContentPanelFileExplorer::init()
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

	_treeView->setCurrentIndex(_fsModel->index(Manager::instance()->projectManager()->currentView()));
	connect(_treeView, &QTreeView::clicked, this, &ContentPanelFileExplorer::onModelIndexClicked);

	ContentPanelBase::init();
}

QToolBar *qtengine::ContentPanelFileExplorer::initToolBar()
{
	auto toolbar = new QToolBar(this);

	toolbar->addAction(QIcon(":icon_collapse"), "", _treeView, &QTreeView::collapseAll);
	toolbar->addAction(QIcon(":icon_expand"), "", _treeView, &QTreeView::expandAll);
	toolbar->addSeparator();
	toolbar->addAction(QIcon(":icon_create"), "", this, &ContentPanelFileExplorer::onCreateFile);
	toolbar->addAction(QIcon(":icon_rename"), "", this, &ContentPanelFileExplorer::onRenameFile);
	toolbar->addAction(QIcon(":icon_delete"), "", this, &ContentPanelFileExplorer::onDeleteFile);
	return toolbar;
}

void qtengine::ContentPanelFileExplorer::onModelIndexClicked(const QModelIndex &modelIndex)
{
	Manager::instance()->projectManager()->setCurrentView(_fsModel->filePath(modelIndex));
}

void qtengine::ContentPanelFileExplorer::onCreateFile()
{
	// Ask for create file based on QWidget/QDialog
}

void qtengine::ContentPanelFileExplorer::onRenameFile()
{
	auto index = _treeView->currentIndex();

	if (index.isValid())
		_treeView->edit(index);
}

void qtengine::ContentPanelFileExplorer::onDeleteFile()
{
	auto index = _treeView->currentIndex();

	if (index.isValid())
		_fsModel->remove(index);
}
