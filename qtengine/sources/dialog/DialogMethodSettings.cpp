/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogMethodSettings
*/

#include "DialogMethodSettings.hpp"

qtengine::DialogMethodSettings::DialogMethodSettings(const types::Method &method, QWidget *parent)
	: DialogBase(parent)
	, _method(method)
{
	_mainLayout->insertWidget(0, initBody());

	_buttonBox->addButton(QDialogButtonBox::Cancel);
	_buttonBox->addButton(QDialogButtonBox::Ok);

	setWindowTitle("Method settings");
}

qtengine::DialogMethodSettings::DialogMethodSettings(QWidget *parent)
	: DialogMethodSettings(types::Method(), parent)
{
}

QWidget *qtengine::DialogMethodSettings::initBody()
{
	auto mainWidget = new QWidget(this);
	/*auto mainLayout = */new QVBoxLayout(mainWidget);

	return mainWidget;
}
