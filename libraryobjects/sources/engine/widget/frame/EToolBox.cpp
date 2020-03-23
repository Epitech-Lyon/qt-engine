/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EToolBox
*/

#include "EToolBox.hpp"
#include "EWidget.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"
#include "ObjectManager.hpp"
#include "QVariantConverter.hpp"
#include <QtCore/QJsonArray>

#include <QtWidgets/QWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtCore/QDir>

template<> void libraryObjects::EToolBox::init(AObject *object)
{
	EWidget::init(object);
}

template<> QJsonObject libraryObjects::EToolBox::serializeData(AObject *object)
{
	auto toolBox = dynamic_cast<QToolBox*>(object->object());
	if (!toolBox) { return QJsonObject(); }

	QJsonArray jsonItems;
	for (int i = 0; i < toolBox->count(); i += 1) {
		QJsonObject jsonItemsObj;

		jsonItemsObj["Text"] = toolBox->itemText(i);
		jsonItemsObj["ToolTip"] = toolBox->itemToolTip(i);
		jsonItemsObj["Icon"] = QVariantConverter::serialize(toolBox->itemIcon(i));
		jsonItemsObj["IsEnabled"] = toolBox->isItemEnabled(i);
		jsonItems.append(jsonItemsObj);
	}

	QJsonObject json;
	json["Items"] = jsonItems;
	return json;
}

template<> void libraryObjects::EToolBox::deserializeData(const QJsonObject &json, AObject *object)
{
	auto toolBox = dynamic_cast<QToolBox*>(object->object());
	if (!toolBox) { return; }

	int index = 0;
	for (auto jsonItemsObjRef : json["Items"].toArray()) {
		QJsonObject jsonItemsObj = jsonItemsObjRef.toObject();

		toolBox->setItemText(index, jsonItemsObj["Text"].toString());
		toolBox->setItemToolTip(index, jsonItemsObj["ToolTip"].toString());
		toolBox->setItemIcon(index, QVariantConverter::deserialize(jsonItemsObj["Icon"].toString()).value<QIcon>());
		toolBox->setItemEnabled(index, jsonItemsObj["IsEnabled"].toBool());
		index += 1;
	}
}

template<> QString libraryObjects::EToolBox::classIncludePath()
{
	return "#include <QtWidgets/QToolBox>";
}

template<> QIcon libraryObjects::EToolBox::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EToolBox::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QWidget>::classHierarchy(), LibraryFunction::FunctionDrag("insertWidget", ToolBox::insertItem, "removeWidget", ToolBox::removeItem));
	libraryFunction->addFunctionMenuChildren(LibraryFunction::FunctionMenu("setText", ToolBox::setText));
	libraryFunction->addFunctionMenuChildren(LibraryFunction::FunctionMenu("setToolTip", ToolBox::setToolTip));
	libraryFunction->addFunctionMenuChildren(LibraryFunction::FunctionMenu("setIcon", ToolBox::setIcon));
	libraryFunction->addFunctionMenuChildren(LibraryFunction::FunctionMenu("setEnable", ToolBox::setEnable));
	return libraryFunction;
}

template<> QString libraryObjects::EToolBox::code(AObject *object)
{
	QString code = EWidget::code(object);

	auto toolBox = dynamic_cast<QToolBox*>(object->object());
	if (!toolBox) { return code; }

	for (int i = 0; i < toolBox->count(); i += 1) {
		if (i)
			code += "\n";
		code += ObjectManager::instance()->objectName(object->id()) + "->addItem(" + ObjectManager::instance()->objectName(object->children().at(i)->id()) + ", " + QVariantConverter::toString(toolBox->itemText(i)) + ");\n";
		code += ObjectManager::instance()->objectName(object->id()) + "->setItemToolTip(" + QString::number(i) + ",  " + QVariantConverter::toString(toolBox->itemToolTip(i)) + ");\n";
		code += ObjectManager::instance()->objectName(object->id()) + "->setItemIcon(" + QString::number(i) + ",  QIcon(" + QVariantConverter::toString(object->dynamicProperty("Toolbox::" + QString::number(i) + "::itemIcon", "")) + "));\n";
		code += ObjectManager::instance()->objectName(object->id()) + "->setItemEnabled(" + QString::number(i) + ",  " + QVariantConverter::toString(toolBox->isItemEnabled(i)) + ");\n";
	}
	return code;
}

bool libraryObjects::ToolBox::insertItem(AObject *parent, int index, AObject *child)
{
	auto toolBox = dynamic_cast<QToolBox*>(parent->object());
	if (!toolBox) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (toolBox->indexOf(widget) != -1) { return false; }

	toolBox->insertItem(index, widget, "title");
	parent->insertChild(index, child);
	return true;
}

bool libraryObjects::ToolBox::removeItem(AObject *parent, AObject *child)
{
	auto toolBox = dynamic_cast<QToolBox*>(parent->object());
	if (!toolBox) { return false; }

	auto widget = dynamic_cast<QWidget*>(child->object());
	if (!widget) { return false; }

	if (toolBox->indexOf(widget) == -1) { return false; }

	toolBox->removeItem(toolBox->indexOf(widget));
	parent->removeChild(child);
	return true;
}

void libraryObjects::ToolBox::setText(AObject *object)
{
	auto parent = object->parent();
	if (!parent) { return; }

	auto toolBox = dynamic_cast<QToolBox*>(parent->object());
	if (!toolBox) { return; }

	int index = toolBox->indexOf(dynamic_cast<QWidget*>(object->object()));
	if (index == -1) { return; }

	bool ok = false;
	auto text = QInputDialog::getText(nullptr, "Toolbox setText", "Please enter the text for the selected item", QLineEdit::Normal, "", &ok);
	if (!ok || text.isEmpty()) { return; }

	toolBox->setItemText(index, text);
}

void libraryObjects::ToolBox::setToolTip(AObject *object)
{
	auto parent = object->parent();
	if (!parent) { return; }

	auto toolBox = dynamic_cast<QToolBox*>(parent->object());
	if (!toolBox) { return; }

	int index = toolBox->indexOf(dynamic_cast<QWidget*>(object->object()));
	if (index == -1) { return; }

	bool ok = false;
	auto toolTip = QInputDialog::getText(nullptr, "Toolbox setToolTip", "Please enter the toolTip for the selected item", QLineEdit::Normal, "", &ok);
	if (!ok || toolTip.isEmpty()) { return; }

	toolBox->setItemToolTip(index, toolTip);
}

void libraryObjects::ToolBox::setIcon(AObject *object)
{
	auto parent = object->parent();
	if (!parent) { return; }

	auto toolBox = dynamic_cast<QToolBox*>(parent->object());
	if (!toolBox) { return; }

	int index = toolBox->indexOf(dynamic_cast<QWidget*>(object->object()));
	if (index == -1) { return; }

	auto iconPath = QFileDialog::getOpenFileName(nullptr, "Toolbox setIcon", QDir::homePath(), "Image Files (*.png *.jpg *.bmp)");
	if (iconPath.isEmpty()) { return; }

	parent->addDynamicProperty("Toolbox::" + QString::number(index) + "::itemIcon", iconPath, "");
	toolBox->setItemIcon(index, QIcon(iconPath));
}

void libraryObjects::ToolBox::setEnable(AObject *object)
{
	auto parent = object->parent();
	if (!parent) { return; }

	auto toolBox = dynamic_cast<QToolBox*>(parent->object());
	if (!toolBox) { return; }

	int index = toolBox->indexOf(dynamic_cast<QWidget*>(object->object()));
	if (index == -1) { return; }

	bool ok = false;
	QStringList items = {"Enable", "Disable"};
	auto item = QInputDialog::getItem(nullptr, "Toolbox setEnable", "Please choose if yout want enable or disable the selected item", items, !toolBox->isItemEnabled(index), false, &ok);
	if (!ok || item.isEmpty()) { return; }

	toolBox->setItemEnabled(index, item == items.front());
}
