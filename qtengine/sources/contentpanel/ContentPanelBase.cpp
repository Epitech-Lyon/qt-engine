/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelBase
*/

#include "ContentPanelBase.hpp"

qtengine::ContentPanelBase::ContentPanelBase(const QString &name, QWidget *parent)
	: QWidget(parent)
	, _mainLayout(new QVBoxLayout(this))
	, _name(name)
{
	_mainLayout->setMargin(2);
	setLayout(_mainLayout);

	auto toolBar = initToolBar();
	if (toolBar)
		_mainLayout->addWidget(toolBar);
}

QJsonObject qtengine::ContentPanelBase::serialize() const
{
	QJsonObject json;

	json["ContentPanelType"] = _name;
	return json;
}

void qtengine::ContentPanelBase::deserialize(const QJsonObject &json)
{
	_name = json["ContentPanelType"].toString();
}
