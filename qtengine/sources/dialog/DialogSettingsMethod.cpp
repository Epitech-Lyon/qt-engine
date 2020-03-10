/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsMethod
*/

#include "DialogSettingsMethod.hpp"

#include "Utils.hpp"
#include "ComboBoxFilter.hpp"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>

qtengine::DialogSettingsMethod::DialogSettingsMethod(const types::Method &method, const QString &windowTitle, QWidget *parent)
	: DialogBase(windowTitle, parent)
	, _method(method)
{
	_mainLayout->insertWidget(0, initBody());

	_buttonBox->addButton(QDialogButtonBox::Cancel);
	_buttonBox->addButton(QDialogButtonBox::Ok);
}

qtengine::DialogSettingsMethod::DialogSettingsMethod(const QString &windowTitle, QWidget *parent)
	: DialogSettingsMethod(types::Method(), windowTitle, parent)
{
}

QWidget *qtengine::DialogSettingsMethod::initBody()
{
	auto mainWidget = new QWidget(this);
	auto mainLayout = new QVBoxLayout(mainWidget);

	auto cboxAccess = new QComboBox(mainWidget);
	for (auto access : {QMetaMethod::Private, QMetaMethod::Protected, QMetaMethod::Public})
		cboxAccess->addItem(types::accessToString(access));
	cboxAccess->setCurrentIndex(_method.access());
	connect(cboxAccess, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int currentIndex) {
		_method.setAccess(static_cast<QMetaMethod::Access>(currentIndex));
	});
	addWidgetTo(cboxAccess, "Access", mainLayout);

	auto checkBoxIsStatic = new QCheckBox(mainWidget);
	checkBoxIsStatic->setCheckState(_method.isStatic() ? Qt::Checked : Qt::Unchecked);
	connect(checkBoxIsStatic, &QCheckBox::stateChanged, [this](int state) {
		_method.setStatic(state == Qt::Checked);
	});
	addWidgetTo(checkBoxIsStatic, "Is static", mainLayout);

	auto cboxReturnType = new ComboBoxFilter(mainWidget);
	QStringList cboxReturnTypeItems;
	QList<QVariant> cboxReturnTypeDatas;
	for (int i = 1; i < QMetaType::User; i += 1)
		if (QMetaType::isRegistered(i)) {
			cboxReturnTypeItems.append(QMetaType::typeName(i));
			cboxReturnTypeDatas.append(i);
		}
	cboxReturnType->setItems(cboxReturnTypeItems);
	cboxReturnType->setDatas(cboxReturnTypeDatas);
	cboxReturnType->setCurrentIndex(cboxReturnTypeDatas.indexOf(static_cast<int>(_method.returnType())));
	connect(cboxReturnType, &ComboBoxFilter::currentDataChanged, [this](const QVariant &currentData) {
		_method.setReturnType(static_cast<QMetaType::Type>(currentData.toInt()));
	});
	addWidgetTo(cboxReturnType, "Return type", mainLayout);

	auto leName = new QLineEdit(mainWidget);
	leName->setText(_method.name());
	connect(leName, &QLineEdit::textChanged, [this](const QString &text) { _method.setName(text); });
	addWidgetTo(leName, "Name", mainLayout);

	auto widgetParameters = new QWidget(mainWidget);
	widgetParameters->setVisible(false);
	_layoutParameters = new QVBoxLayout(widgetParameters);
	_layoutParameters->setSpacing(_layoutParameters->contentsMargins().top() * 2);
	_layoutParameters->setContentsMargins(0, _layoutParameters->spacing() - mainLayout->spacing(), 0, _layoutParameters->spacing() - mainLayout->spacing());
	mainLayout->addWidget(widgetParameters);

	auto checkBoxIsConst = new QCheckBox(mainWidget);
	checkBoxIsConst->setCheckState(_method.isConst() ? Qt::Checked : Qt::Unchecked);
	connect(checkBoxIsConst, &QCheckBox::stateChanged, [this](int state) {
		_method.setConst(state == Qt::Checked);
	});
	addWidgetTo(checkBoxIsConst, "Is const", mainLayout);

	auto btnAddParameter = new QPushButton("Add parameter", mainWidget);
	connect(btnAddParameter, &QPushButton::clicked, [this]() {
		QString parameterName = "variable";

		QStringList parametersName;
		for (auto &parameter : _method.parameters())
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

void qtengine::DialogSettingsMethod::addParameter(QMetaType::Type parameterType, const QString &parameterName)
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

	_method.addParameter(parameterType, parameterName);
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

		if (!_method.modifyParameterType(index, static_cast<QMetaType::Type>(currentData.toInt())))
			cboxType->setCurrentText(QMetaType::typeName(_method.parameter(index).first));
	});
	cboxType->setCurrentText(QMetaType::typeName(parameterType));
	addChildWidgetTo(cboxType, "Type", mainLayout);

	auto leName = new QLineEdit(mainWidget);
	connect(leName, &QLineEdit::textChanged, [this, mainWidget, leName](const QString &text) {
		int index = _layoutParameters->indexOf(mainWidget);

		if (!_method.modifyParameterName(index, text))
			leName->setText(_method.parameter(index).second);
	});
	leName->setText(parameterName);
	addChildWidgetTo(leName, "Name", mainLayout);

	auto btnRemove = new QPushButton("Remove", mainWidget);
	connect(btnRemove, &QPushButton::clicked, [this, mainWidget]() {
		_method.removeParameter(_layoutParameters->indexOf(mainWidget));
		_layoutParameters->removeWidget(mainWidget);
		if (_layoutParameters->isEmpty())
			_layoutParameters->parentWidget()->setVisible(false);
		delete mainWidget;
	});
	addChildWidgetTo(btnRemove, "", mainLayout);
}
