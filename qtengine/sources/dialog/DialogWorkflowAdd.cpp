/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogWorkflowAdd
*/

#include "moc_DialogWorkflowAdd.cpp"
#include "DialogWorkflowAdd.hpp"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtCore/QList>
#include <QtCore/QMetaEnum>

qtengine::DialogWorkflowAdd::DialogWorkflowAdd(QWidget *parent)
	: DialogBase(parent)
	, _access(Types::Public)
	, _type(Types::Function)
{
	_mainLayout->insertWidget(0, initBody());

	_buttonBox->addButton(QDialogButtonBox::Cancel);
	_buttonBox->addButton(QDialogButtonBox::Ok);

	setWindowTitle("Workflow add");
}

QWidget *qtengine::DialogWorkflowAdd::initBody()
{
	auto mainWidget = new QWidget(this);
	auto mainLayout = new QVBoxLayout(mainWidget);
	mainLayout->setMargin(0);
	mainWidget->setLayout(mainLayout);

	auto createComboBox = [this, mainLayout](const QMetaEnum &metaEnum, int *var)
	{
		auto comboBox = new QComboBox(mainLayout->parentWidget());
		connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [var](int index) { *var = index; });
		for (int i = 0; i < metaEnum.keyCount(); i += 1)
			comboBox->addItem(metaEnum.key(i));

		addWidgetTo(comboBox, metaEnum.enumName(), mainLayout);
	};

	createComboBox(QMetaEnum::fromType<Types::Access>(), reinterpret_cast<int*>(&_access));
	createComboBox(QMetaEnum::fromType<Types::Type>(), reinterpret_cast<int*>(&_type));

	auto lineEditName = new QLineEdit(mainWidget);
	connect(lineEditName, &QLineEdit::textChanged, [this](const QString &text) { _name = text; });
	addWidgetTo(lineEditName, "Name", mainLayout);

	return mainWidget;
}
