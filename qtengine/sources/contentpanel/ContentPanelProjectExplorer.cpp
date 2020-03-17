/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelProjectExplorer
*/

#include "ContentPanelProjectExplorer.hpp"

#include "TreeWidgetProjectExplorer.hpp"

#include "Manager.hpp"
#include "ProjectManager.hpp"
#include "ViewManager.hpp"

qtengine::ContentPanelProjectExplorer::ContentPanelProjectExplorer(QWidget *parent)
	: ContentPanelBase("Project Explorer", parent)
{
}

void qtengine::ContentPanelProjectExplorer::init()
{
	_tree = new TreeWidgetProjectExplorer(this);
	_tree->setEditTriggers(QTreeWidget::NoEditTriggers);
	_mainLayout->addWidget(_tree);

	onViewsChanged(Manager::instance()->projectManager()->views());
	connect(Manager::instance()->projectManager(), &ProjectManager::viewsChanged, this, &ContentPanelProjectExplorer::onViewsChanged);
	connect(_tree, &TreeWidgetProjectExplorer::viewDoubleClicked, Manager::instance()->viewManager(), &ViewManager::onOpenView);

	ContentPanelBase::init();
}

QToolBar *qtengine::ContentPanelProjectExplorer::initToolBar()
{
	auto toolbar = new QToolBar(this);
	QList<QAction *> actions;

	actions << toolbar->addAction(QIcon(":icon_collapse"), "", _tree, &QTreeWidget::collapseAll);
	actions << toolbar->addAction(QIcon(":icon_expand"), "", _tree, &QTreeWidget::expandAll);
	toolbar->addSeparator();
	actions << toolbar->addAction(QIcon(":icon_create"), "", Manager::instance()->projectManager(), &ProjectManager::onCreateView);
	actions << toolbar->addAction(QIcon(":icon_import"), "", Manager::instance()->projectManager(), &ProjectManager::onImportView);
//	actions << toolbar->addAction(QIcon(":icon_rename"), "", this, &ContentPanelProjectExplorer::onRenameFile);
	actions << toolbar->addAction(QIcon(":icon_remove"), "", [this]() {
		Manager::instance()->projectManager()->onRemoveView(_tree->currentView());
		Manager::instance()->viewManager()->closeView();
	});

	auto onProjectChanged = [actions](bool isOpened) {
		for (auto action : actions)
			action->setEnabled(isOpened);
	};

	onProjectChanged(Manager::instance()->projectManager()->projectIsOpened());
	connect(Manager::instance()->projectManager(), &ProjectManager::projectOpened, this, onProjectChanged);
	return toolbar;
}

void qtengine::ContentPanelProjectExplorer::onViewsChanged(const QStringList &viewsPath)
{
	_tree->clearViews();
	for (auto view : viewsPath)
		_tree->addView(view);
	_tree->setCurrentView(Manager::instance()->viewManager()->viewPath());
}

void qtengine::ContentPanelProjectExplorer::onRenameFile()
{
}
