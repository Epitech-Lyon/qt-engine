/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogBase
*/

#include "DialogBase.hpp"
#include <QtWidgets/QSpacerItem>

qtengine::DialogBase::DialogBase(QWidget *parent)
	: QDialog(parent)
{
	_mainLayout = new QVBoxLayout(this);
	_mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(_mainLayout);

	_mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

	_buttonBox = new QDialogButtonBox(this);
	connect(_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	_mainLayout->addWidget(_buttonBox);
}

void qtengine::DialogBase::addWidgetTo(QWidget *widgetToAdd, const QString &name, QLayout *layoutParent)
{
	auto widget = new QWidget(layoutParent->parentWidget());
	widgetToAdd->setParent(widget);
	layoutParent->addWidget(widget);

	_labels << new QLabel(name, widget);

	auto layout = new QHBoxLayout(widget);
	layout->setMargin(0);
	layout->addWidget(_labels.last());
	layout->addWidget(widgetToAdd);
	widget->setLayout(layout);

	resizeWidgets();
}

void qtengine::DialogBase::resizeWidgets()
{
	int maxWidth = 0;

	for (auto label : _labels)
		maxWidth = qMax(maxWidth, label->minimumSizeHint().width());
	for (auto label : _labels)
		label->setFixedWidth(maxWidth * 1.25);
}
