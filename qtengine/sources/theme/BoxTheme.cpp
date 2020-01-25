/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** BoxTheme
*/

#include "BoxTheme.hpp"

#include "OrangeDark.hpp"
#include "BlueDark.hpp"
#include "Dark.hpp"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>

qtengine::BoxTheme::BoxTheme(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle("choose a theme");

	auto layout = new QGridLayout;
	setLayout(layout);

	auto comboBox = new QComboBox;
	comboBox->addItem("Light");
	comboBox->addItem("Blue dark");
	comboBox->addItem("Orange dark");
	comboBox->addItem("Dark");
	layout->addWidget(comboBox, 0, 0, 1, 3);

	auto btnOk = new QPushButton(tr("Ok"));
	connect(btnOk, &QPushButton::released, [this, comboBox]() { applyTheme(comboBox->currentText()); });
	connect(btnOk, &QPushButton::released, this, &QDialog::accept);
	layout->addWidget(btnOk, 1, 0, 1, 1);

	auto btnCancel = new QPushButton(tr("Cancel"));
	connect(btnCancel, &QPushButton::released, this, &QDialog::accept);
	layout->addWidget(btnCancel, 1, 1, 1, 1);

	auto btnApply = new QPushButton(tr("Apply"));
	connect(btnApply, &QPushButton::released, [this, comboBox]() { applyTheme(comboBox->currentText()); });
	layout->addWidget(btnApply, 1, 2, 1, 1);
}

void qtengine::BoxTheme::applyTheme(const QString &newTheme)
{
	_theme = newTheme;
	if (newTheme == "Light")
		themeLight();
	else if (newTheme == "Blue dark")
		themeBlueDark();
	else if (newTheme == "Orange dark")
		themeOrangeDark();
	else if (newTheme == "Dark")
		themeDark();
}

void qtengine::BoxTheme::themeLight()
{
	qApp->setPalette(this->style()->standardPalette());
	qApp->setStyle(QStyleFactory::create("fusion"));
	qApp->setStyleSheet("");
}

void qtengine::BoxTheme::themeBlueDark()
{
	qApp->setStyle(new BlueDark);
}

void qtengine::BoxTheme::themeOrangeDark()
{
	qApp->setStyle(new OrangeDark);
}

void qtengine::BoxTheme::themeDark()
{
	qApp->setStyle(new Dark);
}
