/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelFileExplorer
*/

#include "ContentPanelFileExplorer.hpp"

#include "Manager.hpp"
#include "ProjectManager.hpp"

#include <QtWidgets/QTreeView>
#include <QtWidgets/QFileSystemModel>

qtengine::ContentPanelFileExplorer::ContentPanelFileExplorer(QWidget *parent)
	: ContentPanelBase("File Explorer", parent)
{
}

void qtengine::ContentPanelFileExplorer::init()
{
	ContentPanelBase::init();

	auto treeView = new QTreeView(this);
	auto fileSystem = new QFileSystemModel(treeView);
	auto onProjectDirChanged = [treeView, fileSystem](const QString &projectDir) {
		if (projectDir.isEmpty())
			treeView->setModel(nullptr);
		else {
			treeView->setModel(fileSystem);
			treeView->setRootIndex(fileSystem->setRootPath(projectDir + "/views"));
		}
	};

	fileSystem->setReadOnly(true);
	treeView->setModel(fileSystem);
	_mainLayout->addWidget(treeView);

	onProjectDirChanged(Manager::instance()->projectManager()->projectDir());
	connect(Manager::instance()->projectManager(), &ProjectManager::projectDirChanged, onProjectDirChanged);
}
