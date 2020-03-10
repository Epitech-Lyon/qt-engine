/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogBase
*/

#include "DialogBase.hpp"
#include <QtWidgets/QSpacerItem>

qtengine::DialogBase::DialogBase(const QString &windowTitle, QWidget *parent)
	: QDialog(parent)
	, _labelsWidth(0)
{
	_mainLayout = new QVBoxLayout(this);
	_mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(_mainLayout);

	_mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

	_buttonBox = new QDialogButtonBox(this);
	connect(_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	_mainLayout->addWidget(_buttonBox);

	setWindowTitle(windowTitle);
}

void qtengine::DialogBase::addWidgetTo(QWidget *widgetToAdd, const QString &name, QBoxLayout *layoutParent)
{
	insertWidgetTo(layoutParent->count(), widgetToAdd, name, layoutParent);
}

void qtengine::DialogBase::insertWidgetTo(int index, QWidget *widgetToAdd, const QString &name, QBoxLayout *layoutParent)
{
	auto widget = new QWidget(layoutParent->parentWidget());
	widgetToAdd->setParent(widget);
	layoutParent->insertWidget(index, widget);

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
	_labelsWidth = 0;
	for (auto label : _labels)
		_labelsWidth = qMax(_labelsWidth, label->minimumSizeHint().width());
	for (auto label : _labels)
		label->setFixedWidth(_labelsWidth * 1.25);
}
