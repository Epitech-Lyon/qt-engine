/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TreeWidgetWorkflow
*/

#include "TreeWidgetWorkflow.hpp"

#include "ObjectClass.hpp"
#include "DialogSettingsConstructor.hpp"
#include "DialogSettingsMethod.hpp"
#include "DialogSettingsProperty.hpp"
#include "Utils.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "MainWindow.hpp"

#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

qtengine::TreeWidgetWorkflow::TreeWidgetWorkflow(QWidget *parent)
	: QTreeWidget(parent)
	, _viewObjectClass(nullptr)
{
	viewport()->setAutoFillBackground(false);
	setFrameShape(QFrame::NoFrame);

	setHeaderHidden(true);
	setColumnCount(2);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
	header()->setSectionResizeMode(1, QHeaderView::Fixed);
	header()->setStretchLastSection(false);
	clear();
}

void qtengine::TreeWidgetWorkflow::clear()
{
	auto initBranch = [this](const QString &branchName, std::function<void ()> signalToEmit) {
		auto parentItem = new QTreeWidgetItem(invisibleRootItem(), { branchName });
		parentItem->setFlags(Qt::ItemIsEnabled);

		auto btnAdd = new QPushButton("+");
		connect(btnAdd, &QPushButton::clicked, btnAdd, [signalToEmit]() { emit signalToEmit(); });
		setItemWidget(parentItem, 1, btnAdd);
		setColumnWidth(1, btnAdd->height());

		QMap<QMetaMethod::Access, QTreeWidgetItem *> items;
		for (auto access : {QMetaMethod::Private, QMetaMethod::Protected, QMetaMethod::Public}) {
			items[access] = new QTreeWidgetItem(parentItem, { types::accessToString(access) });
			items[access]->setFlags(Qt::ItemIsEnabled);
		}
		return items;
	};

	QTreeWidget::clear();
	_itemsConstructor = initBranch("Constructor", std::bind(&TreeWidgetWorkflow::onAddConstructorClicked, this));
	_itemsMethod = initBranch("Method", std::bind(&TreeWidgetWorkflow::onAddMethodClicked, this));
	_itemsSignal = initBranch("Signal", std::bind(&TreeWidgetWorkflow::onAddSignalClicked, this));
	_itemsSlot = initBranch("Slot", std::bind(&TreeWidgetWorkflow::onAddSlotClicked, this));
	_itemsProperty = initBranch("Property", std::bind(&TreeWidgetWorkflow::onAddPropertyClicked, this));
}

void qtengine::TreeWidgetWorkflow::onViewObjectClassChanged(libraryObjects::ObjectClass *viewObjectClass)
{
	_viewObjectClass = viewObjectClass;
	clear();
}

void qtengine::TreeWidgetWorkflow::onAddConstructorClicked()
{
	DialogSettingsConstructor dialog(Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted) {
		auto constructor = dialog.constructor();

		constructor.setClassName(Manager::instance()->viewManager()->viewName());
		if (constructor.isValid())
			_viewObjectClass->addConstructor(new types::Constructor(constructor));
		_viewObjectClass->addConstructor(new types::Constructor(constructor));
		_viewObjectClass->addConstructor(new types::Constructor(constructor));
	}
}

void qtengine::TreeWidgetWorkflow::onAddMethodClicked()
{
	DialogSettingsMethod dialog("Method settings", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.method().isValid()) {
		_viewObjectClass->addMethod(new types::Method(dialog.method()));
	}
}

void qtengine::TreeWidgetWorkflow::onAddSignalClicked()
{
	DialogSettingsMethod dialog("Signal settings", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.method().isValid()) {
		_viewObjectClass->addSlot(new types::Method(dialog.method()));
	}
}

void qtengine::TreeWidgetWorkflow::onAddSlotClicked()
{
	DialogSettingsMethod dialog("Slot settings", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.method().isValid()) {
		_viewObjectClass->addSignal(new types::Method(dialog.method()));
	}
}

void qtengine::TreeWidgetWorkflow::onAddPropertyClicked()
{
	DialogSettingsProperty dialog(Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.property().isValid()) {
		_viewObjectClass->addProperty(new types::Property(dialog.property()));
	}
}
