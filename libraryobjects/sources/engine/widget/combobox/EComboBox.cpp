/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EComboBox
*/

#include "EComboBox.hpp"
#include "EWidget.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"
#include "ObjectManager.hpp"

#include <QtWidgets/QInputDialog>
#include <QtCore/QJsonArray>

template<> void libraryObjects::EComboBox::init(AObject *object)
{
	auto comboBox = dynamic_cast<QComboBox*>(object->object());

	EObject::init(object);
	connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), object, [object](int index) { emit object->propertyUpdated("currentIndex", index); });
	connect(comboBox, &QComboBox::currentTextChanged, object, [object](const QString &text) { emit object->propertyUpdated("currentText", text); });
}

template<> QJsonObject libraryObjects::EComboBox::serializeData(AObject *object)
{
	auto comboBox = dynamic_cast<QComboBox*>(object->object());
	if (!comboBox) { return QJsonObject(); }

	QJsonArray jsonItems;
	for (int i = 0; i < comboBox->count(); i += 1)
		jsonItems.append(comboBox->itemText(i));

	QJsonObject json;
	json["Items"] = jsonItems;
	return json;
}

template<> void libraryObjects::EComboBox::deserializeData(const QJsonObject &json, AObject *object)
{
	auto comboBox = dynamic_cast<QComboBox*>(object->object());
	if (!comboBox) { return; }

	QStringList items;
	for (auto itemRef : json["Items"].toArray())
		items << itemRef.toString();
	comboBox->addItems(items);
}

template<> QString libraryObjects::EComboBox::classIncludePath()
{
	return "#include <QtWidgets/QComboBox>";
}

template<> QIcon libraryObjects::EComboBox::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EComboBox::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionMenu(LibraryFunction::FunctionMenu("editItems", ComboBox::editItems));
	return libraryFunction;
}

template<> QString libraryObjects::EComboBox::code(AObject *object)
{
	QString code = EWidget::code(object);

	auto comboBox = dynamic_cast<QComboBox*>(object->object());
	if (!comboBox) { return code; }

	for (int i = 0; i < comboBox->count(); i += 1)
		code += ObjectManager::instance()->objectName(object->id()) + "->addItem(\"" + comboBox->itemText(i) + "\");\n";
	return code;
}

void libraryObjects::ComboBox::editItems(AObject *object)
{
	auto comboBox = dynamic_cast<QComboBox*>(object->object());
	if (!comboBox) { return; }

	QStringList items;
	for (int i = 0; i < comboBox->count(); i += 1)
		items << comboBox->itemText(i);

	bool ok = false;
	auto newItems = QInputDialog::getMultiLineText(nullptr, "ComboBox editItems", "Please enter each item in a new line", items.join("\n"), &ok);
	if (!ok || newItems.isEmpty()) { return; }

	items = newItems.split("\n");
	items.removeAll("");

	comboBox->clear();
	comboBox->addItems(items);
}
