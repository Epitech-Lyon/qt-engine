/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** MainWindow
*/

#include "MainWindow.hpp"

#include "LayoutPanelTabber.hpp"
#include "ContentPanelBase.hpp"
#include "BoxTheme.hpp"

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QInputDialog>

qtengine::MainWindow::MainWindow()
{
	setenv("FONTCONFIG_PATH", "/etc/fonts", 0);
	setWindowTitle("Qt-Engine");
	statusBar()->showMessage("Status bar");

	initMenuBar();
	initInterface();
}

void qtengine::MainWindow::initMenuBar()
{
	auto menuFile = menuBar()->addMenu("File");
	menuFile->addAction("New project", this, &MainWindow::onNewProject, QKeySequence::New);
	menuFile->addAction("Open project", this, &MainWindow::onOpenProject, QKeySequence::Open);
	menuFile->addAction("Save project", this, &MainWindow::onSaveProject, QKeySequence::Save);
	menuFile->addAction("Save project as", this, &MainWindow::onSaveProjectAs, QKeySequence::SaveAs);
	menuFile->addSeparator();
	menuFile->addAction("Exit", this, &QMainWindow::close, QKeySequence::Quit);

	auto menuSettings = menuBar()->addMenu("Settings");
	menuSettings->addAction("Theme", this, &MainWindow::onTheme);

	_menuLayouts = new QMenu(this);
	_menuLayoutsSeparator = _menuLayouts->addSeparator();
	_menuLayouts->addAction("Save layout", this, &MainWindow::onSaveLayout);
	_menuLayoutsBtnDelete = _menuLayouts->addAction("Delete layout", this, &MainWindow::onDeleteLayout);
	_menuLayoutsBtnDelete->setEnabled(false);

	auto btnLayouts = new QPushButton("Layouts", this);
	btnLayouts->setMenu(_menuLayouts);
	menuBar()->setCornerWidget(btnLayouts);
}

void qtengine::MainWindow::initInterface()
{
	auto layoutPanelbase = new LayoutPanelBase(this);
	auto layoutPanelTabber = new LayoutPanelTabber(this);

	layoutPanelTabber->addView(new ContentPanelBase("empty", layoutPanelTabber));
	layoutPanelbase->setChildPanel(layoutPanelTabber);
	setCentralWidget(layoutPanelbase);
}

void qtengine::MainWindow::onNewProject()
{
}

void qtengine::MainWindow::onOpenProject()
{
}

void qtengine::MainWindow::onSaveProject()
{
}

void qtengine::MainWindow::onSaveProjectAs()
{
}

void qtengine::MainWindow::onTheme()
{
	BoxTheme(this).exec();
}

QAction *qtengine::MainWindow::createLayoutAction(const QString &layoutName)
{
	auto action = new QAction(layoutName, this);

	return action;
}

QAction *qtengine::MainWindow::getLayoutActionByName(const QString &layoutName) const
{
	QAction *ret = nullptr;

	for (auto action : _menuLayouts->actions())
		if (action->text() == layoutName)
			ret = action;
	return ret;
}

QList<QAction *> qtengine::MainWindow::getLayoutsAction() const
{
	QList<QAction *> ret;
	bool findSeparator = false;

	for (auto action : _menuLayouts->actions()) {
		if (action->isSeparator())
			findSeparator = true;
		else if (!findSeparator)
			ret << action;
	}
	return ret;
}

QStringList qtengine::MainWindow::getLayoutsActionName() const
{
	QStringList ret;
	bool findSeparator = false;

	for (auto action : _menuLayouts->actions()) {
		if (action->isSeparator())
			findSeparator = true;
		else if (!findSeparator)
			ret << action->text();
	}
	return ret;
}

void qtengine::MainWindow::onSaveLayout()
{
	bool ok = false;
	auto layoutsActionName = getLayoutsActionName();
	QString layoutName = QInputDialog::getText(this, "Save Layout", "Choose a name for your Layout", QLineEdit::Normal, "Name", &ok);

	if (ok && !layoutName.isEmpty()) {
		_menuLayouts->insertAction(_menuLayoutsSeparator, createLayoutAction(layoutName));
		_menuLayoutsBtnDelete->setEnabled(true);

//		QJsonObject json; // serialize panel
		// Save to file
	}
}

void qtengine::MainWindow::onDeleteLayout()
{
	bool ok = false;
	auto layoutsActionName = getLayoutsActionName();
	QString layoutNameToDel = QInputDialog::getItem(this, "Delete Layout", "Choose which layout you want to delete", layoutsActionName, 0, false, &ok);

	if (ok && !layoutNameToDel.isEmpty()) {
		auto layoutActionToDel = getLayoutActionByName(layoutNameToDel);

		_menuLayouts->removeAction(layoutActionToDel);
		_menuLayoutsBtnDelete->setEnabled(!(layoutsActionName.size() - 1 == 0));
		// Load json's layout and delete the section who concern the layout deleted.
	}
}
