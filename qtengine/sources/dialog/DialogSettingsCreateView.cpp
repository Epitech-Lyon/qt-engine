/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsCreateView
*/

#include "DialogSettingsCreateView.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "ProjectManager.hpp"

#include "LibraryObjectManager.hpp"

#include <QtWidgets/QFileIconProvider>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>

qtengine::DialogSettingsCreateView::DialogSettingsCreateView(const QString &viewPath, QWidget *parent)
	: DialogBase("Create view", parent)
	, _viewPath(viewPath)
	, _isCopy(false)
	, _libraryObject(nullptr)
{
	_mainLayout->insertWidget(0, initBody());

	_buttonBox->addButton(QDialogButtonBox::Cancel);
	_buttonBox->addButton(QDialogButtonBox::Ok);
}

QWidget *qtengine::DialogSettingsCreateView::initBody()
{
	QWidget *mainWidget = new QWidget(this);
	auto mainLayout = new QVBoxLayout(mainWidget);

	auto widgetViewPath = new QWidget(mainWidget);
	auto lblViewPath = new QLabel(_viewPath, widgetViewPath);
	auto btnViewPath = new QPushButton(QFileIconProvider().icon(QFileIconProvider::Folder), "", widgetViewPath);
	btnViewPath->setFixedSize(btnViewPath->minimumSizeHint());
	connect(btnViewPath, &QPushButton::clicked, [this, lblViewPath]() {
		auto extension = Manager::instance()->viewManager()->viewExtension();
		auto viewPath = QFileDialog::getSaveFileName(this, "Choose view path", _viewPath, "View(*" + extension + ")");

		if (!viewPath.isEmpty()) {
			_viewPath = viewPath.endsWith(extension) ? viewPath : viewPath + extension;
			lblViewPath->setText(_viewPath);
		}
	});
	widgetViewPath->setLayout(new QHBoxLayout(widgetViewPath));
	widgetViewPath->layout()->addWidget(lblViewPath);
	widgetViewPath->layout()->addWidget(btnViewPath);
	widgetViewPath->layout()->setMargin(0);
	addWidgetTo(widgetViewPath, "View path", mainLayout);

	QCheckBox *checkBoxIsCopy = nullptr;
	if (!Manager::instance()->projectManager()->views().isEmpty()) {
		checkBoxIsCopy = new QCheckBox(mainWidget);
		addWidgetTo(checkBoxIsCopy, "Copy", mainLayout);
	}

	auto cBoxLibraryObject = new QComboBox(mainWidget);
	QMap<QString, libraryObjects::LibraryObject *> libraryObjects;
	for (auto libraryObject : libraryObjects::LibraryObjectManager::instance()->libraryObjects())
		libraryObjects[libraryObject->className()] = libraryObject;
	for (auto libraryObject : libraryObjects::LibraryObjectManager::instance()->customObjects())
		libraryObjects[libraryObject->className()] = libraryObject;
	cBoxLibraryObject->addItems(libraryObjects.keys());
	connect(cBoxLibraryObject, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [this, libraryObjects](const QString &className) {
		_libraryObject = libraryObjects[className];
	});
	_libraryObject = libraryObjects["QWidget"];
	cBoxLibraryObject->setCurrentIndex(libraryObjects.keys().indexOf("QWidget"));
	auto widgetCBoxLibraryObject = addWidgetTo(cBoxLibraryObject, "Base class", mainLayout);

	QMap<QString, QString> views;
	for (auto view : Manager::instance()->projectManager()->views())
		views[QFileInfo(view).completeBaseName()] = view;
	if (!views.isEmpty()) {
		auto cBoxCopyFromView = new QComboBox(mainWidget);
		cBoxCopyFromView->addItems(views.keys());
		connect(cBoxCopyFromView, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [this, views](const QString &view) {
			_copyFromView = views[view];
		});
		_copyFromView = views[Manager::instance()->viewManager()->viewName()];
		cBoxCopyFromView->setCurrentIndex(views.keys().indexOf(Manager::instance()->viewManager()->viewName()));
		auto widgetCBoxCopyFromView = addWidgetTo(cBoxCopyFromView, "From", mainLayout);

		auto onIsCopyChanged = [this, widgetCBoxLibraryObject, widgetCBoxCopyFromView](int state) {
			_isCopy = state == Qt::Checked;
			widgetCBoxLibraryObject->setVisible(!_isCopy);
			widgetCBoxCopyFromView->setVisible(_isCopy);
		};
		onIsCopyChanged(checkBoxIsCopy->checkState());
		connect(checkBoxIsCopy, &QCheckBox::stateChanged, onIsCopyChanged);
	}

	return mainWidget;
}
