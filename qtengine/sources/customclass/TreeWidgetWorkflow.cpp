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
#include <QtWidgets/QMenu>

qtengine::TreeWidgetWorkflow::TreeWidgetWorkflow(QWidget *parent)
	: QTreeWidget(parent)
	, _viewObjectClass(nullptr)
{
	viewport()->setAutoFillBackground(false);
	setFrameShape(QFrame::NoFrame);

	setContextMenuPolicy(Qt::CustomContextMenu);
	setHeaderHidden(true);
	setColumnCount(2);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
	header()->setSectionResizeMode(1, QHeaderView::Fixed);
	header()->setStretchLastSection(false);
	clear();

	connect(this, &QTreeWidget::customContextMenuRequested, this, &TreeWidgetWorkflow::onCustomContextMenuRequested);
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
	_childItemsConstructor.clear();
	_childItemsMethod.clear();
	_childItemsSignal.clear();
	_childItemsSlot.clear();
	_childItemsProperty.clear();

	_itemsConstructor = initBranch("Constructor", std::bind(&TreeWidgetWorkflow::onAddConstructorClicked, this));
	_itemsMethod = initBranch("Method", std::bind(&TreeWidgetWorkflow::onAddMethodClicked, this));
	_itemsSignal = initBranch("Signal", std::bind(&TreeWidgetWorkflow::onAddSignalClicked, this));
	_itemsSlot = initBranch("Slot", std::bind(&TreeWidgetWorkflow::onAddSlotClicked, this));
	_itemsProperty = initBranch("Property", std::bind(&TreeWidgetWorkflow::onAddPropertyClicked, this));
}

void qtengine::TreeWidgetWorkflow::onCustomContextMenuRequested(const QPoint &pos)
{
	auto item = itemAt(pos);

	QMenu menu;
	menu.addAction("Settings", [this, item]() {
		if (_childItemsConstructor.contains(item)) { onSettingsConstructorClicked(item, _childItemsConstructor[item]); }
		if (_childItemsMethod.contains(item)) { onSettingsMethodClicked(item, _childItemsMethod[item]); }
		if (_childItemsSignal.contains(item)) { onSettingsSignalClicked(item, _childItemsSignal[item]); }
		if (_childItemsSlot.contains(item)) { onSettingsSlotClicked(item, _childItemsSlot[item]); }
		if (_childItemsProperty.contains(item)) { onSettingsPropertyClicked(item, _childItemsProperty[item]); }
	});
	menu.addAction("Delete", [this, item]() {
		if (_childItemsConstructor.contains(item)) { onDeleteConstructorClicked(item, _childItemsConstructor[item]); }
		if (_childItemsMethod.contains(item)) { onDeleteMethodClicked(item, _childItemsMethod[item]); }
		if (_childItemsSignal.contains(item)) { onDeleteSignalClicked(item, _childItemsSignal[item]); }
		if (_childItemsSlot.contains(item)) { onDeleteSlotClicked(item, _childItemsSlot[item]); }
		if (_childItemsProperty.contains(item)) { onDeletePropertyClicked(item, _childItemsProperty[item]); }
	});
	menu.exec(mapToGlobal(pos));
}

void qtengine::TreeWidgetWorkflow::onViewObjectClassChanged(libraryObjects::ObjectClass *viewObjectClass)
{
	_viewObjectClass = viewObjectClass;
	clear();

	if (!_viewObjectClass) { return; }
	for (auto constructor : _viewObjectClass->getContructors())
		addTypeItem(_itemsConstructor, _childItemsConstructor, constructor);
	for (auto method : _viewObjectClass->getMethods())
		addTypeItem(_itemsMethod, _childItemsMethod, method);
	for (auto signal : _viewObjectClass->getSignals())
		addTypeItem(_itemsSignal, _childItemsSignal, signal);
	for (auto slot : _viewObjectClass->getSlots())
		addTypeItem(_itemsSlot, _childItemsSlot, slot);
	for (auto property : _viewObjectClass->getProperties())
		addTypeItem(_itemsProperty, _childItemsProperty, property);
}

void qtengine::TreeWidgetWorkflow::onAddConstructorClicked()
{
	DialogSettingsConstructor dialog(Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted) {
		auto constructor = new types::Constructor(dialog.constructor());

		constructor->setClassName(Manager::instance()->viewManager()->viewName());
		if (constructor->isValid()) {
			_viewObjectClass->addConstructor(constructor);
			addTypeItem(_itemsConstructor, _childItemsConstructor, constructor);
		} else
			delete constructor;
	}
}

void qtengine::TreeWidgetWorkflow::onAddMethodClicked()
{
	DialogSettingsMethod dialog("Method settings", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.method().isValid()) {
		auto method = new types::Method(dialog.method());

		_viewObjectClass->addMethod(method);
		addTypeItem(_itemsMethod, _childItemsMethod, method);
	}
}

void qtengine::TreeWidgetWorkflow::onAddSignalClicked()
{
	DialogSettingsMethod dialog("Signal settings", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.method().isValid()) {
		auto signal = new types::Method(dialog.method());

		_viewObjectClass->addSignal(signal);
		addTypeItem(_itemsSignal, _childItemsSignal, signal);
	}
}

void qtengine::TreeWidgetWorkflow::onAddSlotClicked()
{
	DialogSettingsMethod dialog("Slot settings", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.method().isValid()) {
		auto slot = new types::Method(dialog.method());

		_viewObjectClass->addSlot(slot);
		addTypeItem(_itemsSlot, _childItemsSlot, slot);
	}
}

void qtengine::TreeWidgetWorkflow::onAddPropertyClicked()
{
	DialogSettingsProperty dialog(Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.property().isValid()) {
		auto property = new types::Property(dialog.property());

		_viewObjectClass->addProperty(property);
		addTypeItem(_itemsProperty, _childItemsProperty, property);
	}
}

void qtengine::TreeWidgetWorkflow::onSettingsConstructorClicked(QTreeWidgetItem *item, types::Constructor *constructor)
{
	DialogSettingsConstructor dialog(*constructor, Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.constructor().isValid()) {
		*constructor = dialog.constructor();
		_childItemsConstructor.remove(item);
		addTypeItem(_itemsConstructor, _childItemsConstructor, constructor);
		delete item;
	}
}

void qtengine::TreeWidgetWorkflow::onSettingsMethodClicked(QTreeWidgetItem *item, types::Method *method)
{
	DialogSettingsMethod dialog(*method, "Method settings", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.method().isValid()) {
		*method = dialog.method();
		_childItemsMethod.remove(item);
		addTypeItem(_itemsMethod, _childItemsMethod, method);
		delete item;
	}
}

void qtengine::TreeWidgetWorkflow::onSettingsSignalClicked(QTreeWidgetItem *item, types::Method *signal)
{
	DialogSettingsMethod dialog(*signal, "Signal settings", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.method().isValid()) {
		*signal = dialog.method();
		_childItemsSignal.remove(item);
		addTypeItem(_itemsSignal, _childItemsSignal, signal);
		delete item;
	}
}

void qtengine::TreeWidgetWorkflow::onSettingsSlotClicked(QTreeWidgetItem *item, types::Method *slot)
{
	DialogSettingsMethod dialog(*slot, "Slot settings", Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.method().isValid()) {
		*slot = dialog.method();
		_childItemsSlot.remove(item);
		addTypeItem(_itemsSlot, _childItemsSlot, slot);
		delete item;
	}
}

void qtengine::TreeWidgetWorkflow::onSettingsPropertyClicked(QTreeWidgetItem *item, types::Property *property)
{
	DialogSettingsProperty dialog(*property, Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && dialog.property().isValid()) {
		*property = dialog.property();
		_childItemsProperty.remove(item);
		addTypeItem(_itemsProperty, _childItemsProperty, property);
		delete item;
	}
}

void qtengine::TreeWidgetWorkflow::onDeleteConstructorClicked(QTreeWidgetItem *item, types::Constructor *constructor)
{
	_viewObjectClass->removeConstructor(constructor);
	_childItemsConstructor.remove(item);
	delete item;
}

void qtengine::TreeWidgetWorkflow::onDeleteMethodClicked(QTreeWidgetItem *item, types::Method *method)
{
	_viewObjectClass->removeMethod(method);
	_childItemsMethod.remove(item);
	delete item;
}

void qtengine::TreeWidgetWorkflow::onDeleteSignalClicked(QTreeWidgetItem *item, types::Method *signal)
{
	_viewObjectClass->removeSignal(signal);
	_childItemsSignal.remove(item);
	delete item;
}

void qtengine::TreeWidgetWorkflow::onDeleteSlotClicked(QTreeWidgetItem *item, types::Method *slot)
{
	_viewObjectClass->removeSlot(slot);
	_childItemsSlot.remove(item);
	delete item;
}

void qtengine::TreeWidgetWorkflow::onDeletePropertyClicked(QTreeWidgetItem *item, types::Property *property)
{
	_viewObjectClass->removeProperty(property);
	_childItemsProperty.remove(item);
	delete item;
}
