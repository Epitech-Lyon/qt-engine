/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogPropertySettings
*/

#include "DialogPropertySettings.hpp"

#include "Utils.hpp"
#include "ComboBoxFilter.hpp"
#include <QtWidgets/QLineEdit>

qtengine::DialogPropertySettings::DialogPropertySettings(const types::Property &property, QWidget *parent)
	: DialogBase(parent)
	, _property(property)
{
	_mainLayout->insertWidget(0, initBody());

	_buttonBox->addButton(QDialogButtonBox::Cancel);
	_buttonBox->addButton(QDialogButtonBox::Ok);

	setWindowTitle("Property settings");
}

qtengine::DialogPropertySettings::DialogPropertySettings(QWidget *parent)
	: DialogPropertySettings(types::Property(), parent)
{
}

QWidget *qtengine::DialogPropertySettings::initBody()
{
	auto mainWidget = new QWidget(this);
	auto mainLayout = new QVBoxLayout(mainWidget);

	auto cboxAccess = new QComboBox(mainWidget);
	for (auto access : {QMetaMethod::Private, QMetaMethod::Protected, QMetaMethod::Public})
		cboxAccess->addItem(types::accessToString(access));
	cboxAccess->setCurrentIndex(_property.access());
	connect(cboxAccess, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int currentIndex) {
		_property.setAccess(static_cast<QMetaMethod::Access>(currentIndex));
	});
	addWidgetTo(cboxAccess, "Access", mainLayout);

	auto cboxType = new ComboBoxFilter(mainWidget);
	QStringList cboxTypeItems;
	QList<QVariant> cboxTypeDatas;
	for (int i = 1; i < QMetaType::User; i += 1)
		if (QMetaType::isRegistered(i)) {
			if (i == QMetaType::Void) { continue; }

			cboxTypeItems.append(QMetaType::typeName(i));
			cboxTypeDatas.append(i);
		}
	cboxType->setItems(cboxTypeItems);
	cboxType->setDatas(cboxTypeDatas);
	cboxType->setCurrentIndex(cboxTypeDatas.indexOf(static_cast<int>(_property.type())));
	connect(cboxType, &ComboBoxFilter::currentDataChanged, [this](const QVariant &currentData) {
		_property.setType(static_cast<QMetaType::Type>(currentData.toInt()));
	});
	addWidgetTo(cboxType, "Type", mainLayout);

	auto leName = new QLineEdit(mainWidget);
	leName->setText(_property.name());
	connect(leName, &QLineEdit::textChanged, [this](const QString &text) { _property.setName(text); });
	addWidgetTo(leName, "Name", mainLayout);

	auto leSetter = new QLineEdit(mainWidget);
	leSetter->setText(_property.setterName());
	connect(leSetter, &QLineEdit::textChanged, [this](const QString &text) { _property.setSetterName(text); });
	addWidgetTo(leSetter, "Setter name", mainLayout);

	auto leGetter = new QLineEdit(mainWidget);
	leGetter->setText(_property.getterName());
	connect(leGetter, &QLineEdit::textChanged, [this](const QString &text) { _property.setGetterName(text); });
	addWidgetTo(leGetter, "Getter name", mainLayout);

	return mainWidget;
}
