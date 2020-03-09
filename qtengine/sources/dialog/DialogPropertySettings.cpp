/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogPropertySettings
*/

#include "DialogPropertySettings.hpp"

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
	/*auto mainLayout = */new QVBoxLayout(mainWidget);

	return mainWidget;
}
