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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFileDialog>
#include <QtCore/QFileInfo>

#include "LibraryObjectManager.hpp"

qtengine::DialogSettingsExport::DialogSettingsExport(const QString &outputPath, const QString &generateMainFrom, bool displayProgress, QWidget *parent)
	: DialogBase("Export settings", parent)
	, _outputPath(outputPath)
	, _generateMainFrom(generateMainFrom)
	, _displayProgress(displayProgress)
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
		auto outputPath = QFileDialog::getExistingDirectory(this, "Choose output path", _outputPath);

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

	auto cBoxGenerateMainFrom = new QComboBox(mainWidget);
	QStringList compatibleViewNames = { "NONE" };
	auto libraryObjectQWidget = libraryObjects::LibraryObjectManager::instance()->libraryObjectOfClassName("QWidget");
	for (auto libraryObject : libraryObjects::LibraryObjectManager::instance()->customObjects())
		if (libraryObjectQWidget && libraryObject->classHierarchy().startsWith(libraryObjectQWidget->classHierarchy()))
			compatibleViewNames.append(libraryObject->className());
	cBoxGenerateMainFrom->addItems(compatibleViewNames);
	connect(cBoxGenerateMainFrom, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [this, compatibleViewNames](const QString &name) {
		_generateMainFrom = name != compatibleViewNames.front() ? name : "";
	});
	if (compatibleViewNames.contains(_generateMainFrom))
		cBoxGenerateMainFrom->setCurrentText(_generateMainFrom);
	else
		_generateMainFrom = "";
	addWidgetTo(cBoxGenerateMainFrom, "Generate main from", mainLayout);

	auto checkBoxDisplayProgress = new QCheckBox(mainWidget);
	checkBoxDisplayProgress->setCheckState(_displayProgress ? Qt::Checked : Qt::Unchecked);
	connect(checkBoxDisplayProgress, &QCheckBox::stateChanged, [this](int state) {
		_displayProgress = state == Qt::Checked;
	});
	addWidgetTo(checkBoxDisplayProgress, "Display progress", mainLayout);

	return mainWidget;
}
