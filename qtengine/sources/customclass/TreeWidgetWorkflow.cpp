/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TreeWidgetWorkflow
*/

#include "moc_TreeWidgetWorkflow.cpp"
#include "TreeWidgetWorkflow.hpp"

#include "ObjectClass.hpp"
#include "DialogSettingsClassType.hpp"
#include "Utils.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "MainWindow.hpp"

#include "AObject.hpp"
#include "MimeDataObject.hpp"
#include "types/includes/Constructor.hpp"
#include "types/includes/Property.hpp"

#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMenu>

qtengine::TreeWidgetWorkflow::TreeWidgetWorkflow(QWidget *parent)
	: QTreeWidget(parent)
	, _objectClass(nullptr)
{
	viewport()->setAutoFillBackground(false);
	setFrameShape(QFrame::NoFrame);

	setAnimated(true);
	setDragDropMode(QAbstractItemView::DragOnly);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setHeaderHidden(true);
	setColumnCount(2);
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
	header()->setSectionResizeMode(1, QHeaderView::Fixed);
	header()->setStretchLastSection(false);
	clear();

	connect(this, &QTreeWidget::customContextMenuRequested, this, &TreeWidgetWorkflow::onCustomContextMenuRequested);
	connect(this, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem *item, int) { if (_childItems.contains(item)) emit classTypeDoubleClicked(_childItems[item]); });
}

void qtengine::TreeWidgetWorkflow::clear()
{
	auto initBranch = [this](const QString &branchName, std::function<void ()> callBack) {
		auto parentItem = new QTreeWidgetItem(invisibleRootItem(), { branchName });
		parentItem->setFlags(Qt::ItemIsEnabled);

		auto btnAdd = new QPushButton("+");
		connect(btnAdd, &QPushButton::clicked, btnAdd, callBack);
		setItemWidget(parentItem, 1, btnAdd);
		setColumnWidth(1, btnAdd->height());

		QMap<QMetaMethod::Access, QTreeWidgetItem *> itemMap;
		for (auto access : {QMetaMethod::Private, QMetaMethod::Protected, QMetaMethod::Public}) {
			itemMap[access] = new QTreeWidgetItem(parentItem, { types::accessToString(access) });
			itemMap[access]->setFlags(Qt::ItemIsEnabled);
		}
		return itemMap;
	};

	QTreeWidget::clear();
	_childItems.clear();

	_items[types::ClassType::CONSTRUCTOR] = initBranch("Constructor", [this]() {
		auto constructor = new types::Constructor();

		constructor->setClassName(Manager::instance()->viewManager()->viewName());
		onAddClicked(constructor);
	});
	_items[types::ClassType::METHOD] = initBranch("Method", [this]() { onAddClicked(types::ClassType::construct(types::ClassType::METHOD)); });
	_items[types::ClassType::SIGNAL] = initBranch("Signal", [this]() { onAddClicked(types::ClassType::construct(types::ClassType::SIGNAL)); });
	_items[types::ClassType::SLOT] = initBranch("Slot", [this]() { onAddClicked(types::ClassType::construct(types::ClassType::SLOT)); });
	_items[types::ClassType::PROPERTY] = initBranch("Property", [this]() { onAddClicked(types::ClassType::construct(types::ClassType::PROPERTY)); });
}

void qtengine::TreeWidgetWorkflow::onCustomContextMenuRequested(const QPoint &pos)
{
	auto item = itemAt(pos);
	if (!item || !_childItems.contains(item)) { return; }

	setCurrentItem(item);
	if (_childItems[item]->type() == types::ClassType::CONSTRUCTOR && dynamic_cast<types::Constructor*>(_childItems[item])->parameters().size() == 0) { return; }

	QMenu menu;
	menu.addAction("Settings", [this, item]() { onSettingsClicked(item); });
	menu.addAction("Delete", [this, item]() { onDeleteClicked(item); });
	menu.exec(mapToGlobal(pos));
}

libraryObjects::AObject *qtengine::TreeWidgetWorkflow::object() const
{
	return _object;
}

void qtengine::TreeWidgetWorkflow::setObject(libraryObjects::AObject *object)
{
	_object = object;
}

libraryObjects::ObjectClass *qtengine::TreeWidgetWorkflow::objectClass() const
{
	return _objectClass;
}

void qtengine::TreeWidgetWorkflow::setObjectClass(libraryObjects::ObjectClass *objectClass)
{
	_objectClass = objectClass;
	clear();

	if (!_objectClass) { return; }

	for (int i = 0; i < QMetaEnum::fromType<types::ClassType::Type>().keyCount(); i += 1)
		for (auto classType : _objectClass->getClassType(static_cast<types::ClassType::Type>(i)))
			addTypeItem(classType);
	selectDefaultConstructor();
	expandAll();
}

void qtengine::TreeWidgetWorkflow::selectDefaultConstructor()
{
	for (auto classType : _objectClass->getClassType(types::ClassType::CONSTRUCTOR))
		if (dynamic_cast<types::Constructor*>(classType)->parameters().size() == 0) {
			setCurrentItem(_childItems.key(classType));
			emit classTypeDoubleClicked(classType);
		}
}

void qtengine::TreeWidgetWorkflow::addTypeItem(types::ClassType *classType)
{
	auto parentItem = _items[classType->type()][classType->access()];
	auto item = new QTreeWidgetItem(parentItem, { classType->signature() });

	emit classTypeAdded(classType);
	setCurrentItem(item);
	_childItems[item] = classType;
}

void qtengine::TreeWidgetWorkflow::onAddClicked(types::ClassType *classType)
{
	DialogSettingsClassType dialog(classType, Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && classType->isValid()) {
		auto classTypeRet = _objectClass->addClassType(classType);

		if (classType == classTypeRet) {
			addTypeItem(classType);
		} else {
			delete classType;
			if (classTypeRet)
				setCurrentItem(_childItems.key(classTypeRet));
		}
	} else
		delete classType;
}

void qtengine::TreeWidgetWorkflow::onSettingsClicked(QTreeWidgetItem *item)
{
	auto classType = _childItems[item];
	auto classTypeSave = classType->serialize();
	DialogSettingsClassType dialog(classType, Manager::instance()->mainWindow());

	if (dialog.exec() == QDialog::Accepted && classType->isValid()) {
		_childItems.remove(item);
		addTypeItem(classType);
		delete item;
	} else
		classType->deserialize(classTypeSave);
}

void qtengine::TreeWidgetWorkflow::onDeleteClicked(QTreeWidgetItem *item)
{
	auto classType = _childItems[item];

	emit classTypeDeleted(classType);
	_objectClass->removeClassType(classType);
	_childItems.remove(item);
	delete item;
	delete classType;
}

QMimeData *qtengine::TreeWidgetWorkflow::mimeData(const QList<QTreeWidgetItem *> items) const
{
	auto classTypeSelected = _childItems[items.front()];

	auto objectClass = new libraryObjects::ObjectClass();
	auto classType = types::ClassType::construct(classTypeSelected->type());
	classType->deserialize(classTypeSelected->serialize());
	objectClass->addClassType(classType);

	QMimeData *mimeData = QTreeWidget::mimeData(items);
	auto mimeDataObject = new MimeDataObject(objectClass, nullptr, classType->type() == types::ClassType::CONSTRUCTOR ? nullptr : _object);
	for (auto format : mimeData->formats())
		mimeDataObject->setData(format, mimeData->data(format));
	return mimeDataObject;
}
