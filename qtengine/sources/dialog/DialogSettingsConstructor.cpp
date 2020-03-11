/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsConstructor
*/

#include "DialogSettingsConstructor.hpp"

#include "Utils.hpp"
#include "ComboBoxFilter.hpp"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>

qtengine::DialogSettingsConstructor::DialogSettingsConstructor(const types::Constructor &constructor, QWidget *parent)
	: DialogBase("Constructor settings", parent)
	, _constructor(constructor)
{
	_mainLayout->insertWidget(0, initBody());

	_buttonBox->addButton(QDialogButtonBox::Cancel);
	_buttonBox->addButton(QDialogButtonBox::Ok);
}

qtengine::DialogSettingsConstructor::DialogSettingsConstructor(QWidget *parent)
	: DialogSettingsConstructor(types::Constructor(), parent)
{
}

QWidget *qtengine::DialogSettingsConstructor::initBody()
{
	auto mainWidget = new QWidget(this);
	auto mainLayout = new QVBoxLayout(mainWidget);

	auto cboxAccess = new QComboBox(mainWidget);
	for (auto access : {QMetaMethod::Private, QMetaMethod::Protected, QMetaMethod::Public})
		cboxAccess->addItem(types::accessToString(access));
	cboxAccess->setCurrentIndex(_constructor.access());
	connect(cboxAccess, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int currentIndex) {
		_constructor.setAccess(static_cast<QMetaMethod::Access>(currentIndex));
	});
	addWidgetTo(cboxAccess, "Access", mainLayout);

	auto widgetParameters = new QWidget(mainWidget);
	widgetParameters->setVisible(false);
	_layoutParameters = new QVBoxLayout(widgetParameters);
	_layoutParameters->setSpacing(_layoutParameters->contentsMargins().top() * 2);
	_layoutParameters->setContentsMargins(0, _layoutParameters->spacing() - mainLayout->spacing(), 0, _layoutParameters->spacing() - mainLayout->spacing());
	mainLayout->addWidget(widgetParameters);

	auto btnAddParameter = new QPushButton("Add parameter", mainWidget);
	connect(btnAddParameter, &QPushButton::clicked, [this]() {
		QString parameterName = "variable";

		QStringList parametersName;
		for (auto &parameter : _constructor.parameters())
			parametersName.append(parameter.second);
		if (parametersName.contains(parameterName)) {
			int tmp = 1;
	
			while (parametersName.contains(parameterName + "_" + QString::number(tmp)))
				tmp += 1;
			parameterName = parameterName + "_" + QString::number(tmp);
		}

		addParameter(QMetaType::Int, parameterName);
	});
	mainLayout->addWidget(btnAddParameter);

	return mainWidget;
}

void qtengine::DialogSettingsConstructor::addParameter(QMetaType::Type parameterType, const QString &parameterName)
{
	int labelsWidth = this->labelsWidth();
	auto addChildWidgetTo = [labelsWidth](QWidget *widgetToAdd, const QString &name, QLayout *layoutParent) {
		auto widget = new QWidget(layoutParent->parentWidget());
		widgetToAdd->setParent(widget);
		layoutParent->addWidget(widget);

		auto label = new QLabel(name, widget);
		label->setFixedWidth(labelsWidth);

		auto layout = new QHBoxLayout(widget);
		layout->setContentsMargins(labelsWidth / 4.f, 0, 0, 0);
		layout->addWidget(label);
		layout->addWidget(widgetToAdd);
		widget->setLayout(layout);
	};
	auto mainWidget = new QWidget(_layoutParameters->parentWidget());
	auto mainLayout = new QVBoxLayout(mainWidget);
	mainLayout->setMargin(0);

	_constructor.addParameter(parameterType, parameterName);
	if (_layoutParameters->isEmpty())
		_layoutParameters->parentWidget()->setVisible(true);
	_layoutParameters->addWidget(mainWidget);

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
	connect(cboxType, &ComboBoxFilter::currentDataChanged, [this, mainWidget, cboxType](const QVariant &currentData) {
		int index = _layoutParameters->indexOf(mainWidget);

		if (!_constructor.modifyParameterType(index, static_cast<QMetaType::Type>(currentData.toInt())))
			cboxType->setCurrentText(QMetaType::typeName(_constructor.parameter(index).first));
	});
	cboxType->setCurrentText(QMetaType::typeName(parameterType));
	addChildWidgetTo(cboxType, "Type", mainLayout);

	auto leName = new QLineEdit(mainWidget);
	connect(leName, &QLineEdit::textChanged, [this, mainWidget, leName](const QString &text) {
		int index = _layoutParameters->indexOf(mainWidget);

		if (!_constructor.modifyParameterName(index, text))
			leName->setText(_constructor.parameter(index).second);
	});
	leName->setText(parameterName);
	addChildWidgetTo(leName, "Name", mainLayout);

	auto btnRemove = new QPushButton("Remove", mainWidget);
	connect(btnRemove, &QPushButton::clicked, [this, mainWidget]() {
		_constructor.removeParameter(_layoutParameters->indexOf(mainWidget));
		_layoutParameters->removeWidget(mainWidget);
		if (_layoutParameters->isEmpty())
			_layoutParameters->parentWidget()->setVisible(false);
		delete mainWidget;
	});
	addChildWidgetTo(btnRemove, "", mainLayout);
}
