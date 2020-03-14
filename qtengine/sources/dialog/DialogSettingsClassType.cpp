/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsClassType
*/

#include "DialogSettingsClassType.hpp"

#include "ClassType.hpp"
#include "qtgroupboxpropertybrowser.h"

qtengine::DialogSettingsClassType::DialogSettingsClassType(types::ClassType *classType, QWidget *parent)
	: DialogBase(QString(QMetaEnum::fromType<types::ClassType::Type>().key(classType->type())).toLower() + " settings", parent)
{
	_mainLayout->insertWidget(0, classType->initEditor());

	_buttonBox->addButton(QDialogButtonBox::Cancel);
	_buttonBox->addButton(QDialogButtonBox::Ok);
}
