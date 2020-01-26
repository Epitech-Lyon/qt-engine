/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** MainWindow
*/

#include "MainWindow.hpp"

#include "Manager.hpp"
#include "ProjectManager.hpp"
#include "LayoutManager.hpp"

#include "LayoutPanelTabber.hpp"
#include "LayoutPanelSplitter.hpp"

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>

qtengine::MainWindow::MainWindow(Manager *manager)
	: _manager(manager)
{
	setenv("FONTCONFIG_PATH", "/etc/fonts", 0);
	setWindowTitle("Qt-Engine");
	setAttribute(Qt::WA_DeleteOnClose);

	initMenuBar();
	initInterface();
}

qtengine::MainWindow::~MainWindow()
{
	_manager->save();
}

void qtengine::MainWindow::initMenuBar()
{
	auto projectManager = _manager->projectManager();
	auto layoutManager = _manager->layoutManager();

	auto menuFile = menuBar()->addMenu("File");
	menuFile->addAction("New project", projectManager, &ProjectManager::onNewProject, QKeySequence::New);
	menuFile->addAction("Open project", projectManager, &ProjectManager::onOpenProject, QKeySequence::Open);
	menuFile->addSeparator();
	auto menuFileRecents = menuFile->addMenu("Open recent project");
	menuFile->addAction("Exit", this, &QMainWindow::close, QKeySequence::Quit);

	auto initMenuFileRecents = [projectManager, menuFileRecents](const QStringList &recentsProject) {
		menuFileRecents->clear();
		for (auto recentProject : recentsProject)
			menuFileRecents->addAction(recentProject, [projectManager, recentProject]() { projectManager->openProject(recentProject); });
	};
	connect(projectManager, &ProjectManager::recentProjectsChanged, initMenuFileRecents);
	initMenuFileRecents(projectManager->recentsProject());

	auto menuSettings = menuBar()->addMenu("Settings");
	menuSettings->addAction("Theme", _manager, &Manager::onTheme);

	auto menuLayouts = new QMenu(this);
	auto initMenuLayout = [menuLayouts, layoutManager](const QStringList &keys) {
		menuLayouts->clear();
		for (auto key : keys)
			menuLayouts->addAction(key, [layoutManager, key]() { layoutManager->openLayout(key); });
		menuLayouts->addSeparator();
		menuLayouts->addAction("Save layout", layoutManager, &LayoutManager::onSaveLayout);
		menuLayouts->addAction("Delete layout", layoutManager, &LayoutManager::onDeleteLayout)->setEnabled(keys.size() > 0);
	};

	auto btnLayouts = new QPushButton("Layouts", this);
	btnLayouts->setMenu(menuLayouts);
	menuBar()->setCornerWidget(btnLayouts);

	initMenuLayout(layoutManager->layoutsName());
	connect(layoutManager, &LayoutManager::layoutsNameChanged, initMenuLayout);
}

void qtengine::MainWindow::initInterface()
{
	auto layoutPanelBase = new LayoutPanelBase(this);

	layoutPanelBase->setChild(new LayoutPanelTabber(layoutPanelBase));
	setCentralWidget(layoutPanelBase);
}

QJsonObject qtengine::MainWindow::serialize() const
{
	return dynamic_cast<LayoutPanelBase*>(centralWidget())->child()->serialize();
}

void qtengine::MainWindow::deserialize(const QJsonObject &json)
{
	auto type = json["Type"].toString();
	LayoutPanelBase *base = nullptr;
	
	if (type == "Tabber")
		base = new LayoutPanelTabber;
	else if (type == "Splitter")
		base = new LayoutPanelSplitter(Qt::Horizontal);
	base->deserialize(json["State"].toObject());

	auto centralWidget = dynamic_cast<LayoutPanelBase*>(this->centralWidget());
	delete centralWidget->child();
	centralWidget->setChild(base);
}
