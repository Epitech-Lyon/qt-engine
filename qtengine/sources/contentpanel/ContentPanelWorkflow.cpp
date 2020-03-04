/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#include "moc_ContentPanelWorkflow.cpp"
#include "ContentPanelWorkflow.hpp"

#include "FlowView.hpp"
#include "FlowScene.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "AObject.hpp"
#include "MainWindow.hpp"

#include "DialogWorkflowAdd.hpp"

#include <QtWidgets/QSplitter>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtCore/QDebug>

qtengine::ListWidget::ListWidget(QWidget *parent)
	: QListWidget(parent)
{
	viewport()->setAutoFillBackground(false);
	setFrameShape(QFrame::NoFrame);
}

void qtengine::ListWidget::addFunction(const QString &name)
{
	auto widget = new QLabel("Function " + name, this);

	addItem("");
	setItemWidget(item(count() - 1), widget);
}

void qtengine::ListWidget::addVariable(const QString &name)
{
	auto widget = new QLabel("Variable " + name, this);

	addItem("");
	setItemWidget(item(count() - 1), widget);
}

qtengine::ContentPanelWorkflow::ContentPanelWorkflow(QWidget *parent)
	: ContentPanelBase("Workflow", parent)
{
}

void qtengine::ContentPanelWorkflow::init()
{
	auto splitter = new QSplitter(this);
	_mainLayout->addWidget(splitter);

	auto menuWidget = new QWidget(splitter);
	auto menuLayout = new QVBoxLayout(menuWidget);
	menuLayout->setMargin(0);
	menuLayout->setSpacing(16);
	menuWidget->setLayout(menuLayout);

	auto createMenuFor = [this, menuLayout](const QString &lblText) {
		auto widget = new QWidget(menuLayout->parentWidget());
		menuLayout->addWidget(widget);

		auto label = new QLabel(lblText, widget);
		label->setAlignment(Qt::AlignCenter);

		auto list = new ListWidget(widget);

		auto layout = new QVBoxLayout(widget);
		layout->setMargin(0);
		layout->addWidget(label);
		layout->addWidget(list);

		return list;
	};

	_listPublic = createMenuFor("Public");
	_listProtected = createMenuFor("Protected");
	_listPrivate = createMenuFor("Private");

	auto btnAdd = new QPushButton("Add", menuWidget);
	connect(btnAdd, &QPushButton::clicked, this, &ContentPanelWorkflow::onAddClicked);
	menuLayout->addWidget(btnAdd);

	_scene = new QtNodes::FlowScene();
	_view = new QtNodes::FlowView(_scene, splitter);

	splitter->addWidget(menuWidget);
	splitter->addWidget(_view);
	splitter->setSizes({menuWidget->sizeHint().width(), menuWidget->sizeHint().width() * 4});

	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelWorkflow::onViewObjectChanged);
}

void qtengine::ContentPanelWorkflow::onViewObjectChanged(libraryObjects::AObject *)
{
}

void qtengine::ContentPanelWorkflow::onAddClicked()
{
	DialogWorkflowAdd dialog(Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted) {
		QMap<Types::Access, ListWidget*> listList = {
			{ Types::Public, _listPublic },
			{ Types::Protected, _listProtected },
			{ Types::Private, _listPrivate }};
		QMap<Types::Type, void (ListWidget::*)(const QString &)> listFunctions = {
			{ Types::Function, &ListWidget::addFunction },
			{ Types::Variable, &ListWidget::addVariable }};

		(listList[dialog.access()]->*listFunctions[dialog.type()])(dialog.name());
	}
}
