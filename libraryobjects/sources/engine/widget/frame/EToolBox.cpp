/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EToolBox
*/

#include "EToolBox.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"

#include <QtWidgets/QWidget>
#include <QtWidgets/QInputDialog>
#include <QtCore/QJsonArray>

template<> QJsonObject libraryObjects::EToolBox::serializeData() const
{
	auto toolBox = dynamic_cast<QToolBox*>(object());

	QJsonArray jsonItems;
	for (int i = 0; i < toolBox->count(); i += 1) {
		QJsonObject jsonItemsObj;

// TODO		jsonItemsObj["Icon"] = toolBox->itemIcon(i);
		jsonItemsObj["Text"] = toolBox->itemText(i);
		jsonItemsObj["ToolTip"] = toolBox->itemToolTip(i);
		jsonItemsObj["IsEnabled"] = toolBox->isItemEnabled(i);
		jsonItems.append(jsonItemsObj);
	}

	QJsonObject json;
	json["Items"] = jsonItems;
	return json;
}

template<> void libraryObjects::EToolBox::deserializeData(const QJsonObject &json)
{
	auto toolBox = dynamic_cast<QToolBox*>(object());
	int index = 0;

	for (auto jsonItemsObjRef : json["Items"].toArray()) {
		QJsonObject jsonItemsObj = jsonItemsObjRef.toObject();

// TODO		toolBox->setItemIcon(index, jsonItemsObj["Icon"].toString());
		toolBox->setItemText(index, jsonItemsObj["Text"].toString());
		toolBox->setItemToolTip(index, jsonItemsObj["ToolTip"].toString());
		toolBox->setItemEnabled(index, jsonItemsObj["IsEnabled"].toBool());
		index += 1;
	}
}

template<> QIcon libraryObjects::EToolBox::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EToolBox::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QWidget>::classHierarchy(), LibraryFunction::FunctionDrag("insertWidget", ToolBox::insertItem, "removeWidget", ToolBox::removeItem));
	return libraryFunction;
}

//	bool ok = false;
//	auto title = QInputDialog::getText(nullptr, "insert Item", "Please enter the title of the item", QLineEdit::Normal, "", &ok);
//	if (!ok || title.isEmpty()) { return false; }

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
