/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsExport
*/

#include "DialogSettingsExport.hpp"
#include <QtWidgets/QFileIconProvider>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFileDialog>

qtengine::DialogSettingsExport::DialogSettingsExport(const QString &outputPath, QWidget *parent)
	: DialogBase("Export settings", parent)
	, _outputPath(outputPath)
	, _generateMain(true)
	, _displayProgress(true)
{
	_mainLayout->insertWidget(0, initBody());

	_buttonBox->addButton(QDialogButtonBox::Cancel);
	_buttonBox->addButton(QDialogButtonBox::Ok);
}

QWidget *qtengine::DialogSettingsExport::initBody()
{
	auto mainWidget = new QWidget(this);
	auto mainLayout = new QVBoxLayout(mainWidget);

	auto widgetOutputPath = new QWidget(mainWidget);
	auto lblOutputPath = new QLabel(_outputPath, widgetOutputPath);
	auto btnOutputPath = new QPushButton(QFileIconProvider().icon(QFileIconProvider::Folder), "", widgetOutputPath);
	btnOutputPath->setFixedSize(btnOutputPath->minimumSizeHint());
	connect(btnOutputPath, &QPushButton::clicked, [this, lblOutputPath]() {
		auto outputPath = QFileDialog::getExistingDirectory(nullptr, "Choose output path");

		if (!outputPath.isEmpty()) {
			_outputPath = outputPath;
			lblOutputPath->setText(_outputPath);
		}
	});
	widgetOutputPath->setLayout(new QHBoxLayout(widgetOutputPath));
	widgetOutputPath->layout()->addWidget(lblOutputPath);
	widgetOutputPath->layout()->addWidget(btnOutputPath);
	widgetOutputPath->layout()->setMargin(0);
	addWidgetTo(widgetOutputPath, "Output path", mainLayout);

	auto checkBoxGenerateMain = new QCheckBox(mainWidget);
	checkBoxGenerateMain->setCheckState(_generateMain ? Qt::Checked : Qt::Unchecked);
	connect(checkBoxGenerateMain, &QCheckBox::stateChanged, [this](int state) {
		_generateMain = state == Qt::Checked;
	});
	addWidgetTo(checkBoxGenerateMain, "Generate main", mainLayout);

	auto checkBoxDisplayProgress = new QCheckBox(mainWidget);
	checkBoxDisplayProgress->setCheckState(_displayProgress ? Qt::Checked : Qt::Unchecked);
	connect(checkBoxDisplayProgress, &QCheckBox::stateChanged, [this](int state) {
		_displayProgress = state == Qt::Checked;
	});
	addWidgetTo(checkBoxDisplayProgress, "Display progress", mainLayout);

	return mainWidget;
}
