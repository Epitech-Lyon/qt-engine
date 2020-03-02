/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelProjectExplorer
*/

#include "ContentPanelProjectExplorer.hpp"

#include "Manager.hpp"
#include "ProjectManager.hpp"
#include "ViewManager.hpp"

#include <QtWidgets/QToolBar>
#include <QtWidgets/QLineEdit>

QString qtengine::ContentPanelProjectExplorer::TreeViewItemDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
	return QFileInfo(QStyledItemDelegate::displayText(value, locale)).completeBaseName();
}

void qtengine::ContentPanelProjectExplorer::TreeViewItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QFileInfo fileInfos(index.data().toString());

	const_cast<TreeViewItemDelegate*>(this)->_extension = fileInfos.suffix();
	dynamic_cast<QLineEdit *>(editor)->setText(fileInfos.completeBaseName());
}

void qtengine::ContentPanelProjectExplorer::TreeViewItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	model->setData(index, dynamic_cast<QLineEdit *>(editor)->text() + "." + _extension);
}

qtengine::ContentPanelProjectExplorer::ContentPanelProjectExplorer(QWidget *parent)
	: ContentPanelBase("Project Explorer", parent)
{
}

void qtengine::ContentPanelProjectExplorer::init()
{
	_treeView = new QTreeView(this);
	_fsModel = new QFileSystemModel(_treeView);
	auto onProjectDirChanged = [this](const QString &projectDir) {
		if (projectDir.isEmpty())
			_treeView->setModel(nullptr);
		else {
			_treeView->setModel(_fsModel);
			_treeView->setRootIndex(_fsModel->setRootPath(projectDir + "/views"));
		}
	};

	_fsModel->setNameFilters({"*" + Manager::instance()->viewManager()->viewExtension()});
	_fsModel->setNameFilterDisables(false);
	_fsModel->setReadOnly(false);
	_treeView->setItemDelegate(new TreeViewItemDelegate(_treeView));
	_treeView->setModel(_fsModel);
	_treeView->setHeaderHidden(true);
	_treeView->setDropIndicatorShown(true);
	_treeView->setDragDropMode(QAbstractItemView::InternalMove);
	_treeView->setEditTriggers(QTreeView::NoEditTriggers);
	for (int i = 1; i < _fsModel->columnCount(); ++i)
		_treeView->hideColumn(i);
	_mainLayout->addWidget(_treeView);

	onProjectDirChanged(Manager::instance()->projectManager()->projectDir());
	connect(Manager::instance()->projectManager(), &ProjectManager::projectDirChanged, onProjectDirChanged);

	_treeView->setCurrentIndex(_fsModel->index(Manager::instance()->viewManager()->viewPath()));
	connect(_treeView, &QTreeView::doubleClicked, this, &ContentPanelProjectExplorer::onModelIndexDoubleClicked);

	ContentPanelBase::init();
}

QToolBar *qtengine::ContentPanelProjectExplorer::initToolBar()
{
	auto toolbar = new QToolBar(this);

	toolbar->addAction(QIcon(":icon_collapse"), "", _treeView, &QTreeView::collapseAll);
	toolbar->addAction(QIcon(":icon_expand"), "", _treeView, &QTreeView::expandAll);
	toolbar->addSeparator();
	toolbar->addAction(QIcon(":icon_create"), "", this, &ContentPanelProjectExplorer::onCreateFile);
	toolbar->addAction(QIcon(":icon_rename"), "", this, &ContentPanelProjectExplorer::onRenameFile);
	toolbar->addAction(QIcon(":icon_delete"), "", this, &ContentPanelProjectExplorer::onDeleteFile);
	return toolbar;
}

void qtengine::ContentPanelProjectExplorer::onModelIndexDoubleClicked(const QModelIndex &modelIndex)
{
	Manager::instance()->viewManager()->openView(_fsModel->filePath(modelIndex));
}

void qtengine::ContentPanelProjectExplorer::onCreateFile()
{
	auto connection = new QMetaObject::Connection;
	auto onFileRenamed = [this, connection](const QString &path, const QString &, const QString &newName) {
		Manager::instance()->viewManager()->onCreateView(path + "/" + newName);
		disconnect(*connection);
		delete connection;
	};
	auto path = _fsModel->rootPath() + "/untitled" + Manager::instance()->viewManager()->viewExtension();
	QFile file(path);
	file.open(QIODevice::WriteOnly);
	file.close();

	*connection = connect(_fsModel, &QFileSystemModel::fileRenamed, onFileRenamed);

	auto index = _fsModel->index(path);
	if (index.isValid()) {
		_treeView->setCurrentIndex(index);
		_treeView->edit(index);
	} else {
		QFile::remove(path);
		disconnect(*connection);
		delete connection;
	}
}

void qtengine::ContentPanelProjectExplorer::onRenameFile()
{
	auto index = _treeView->currentIndex();

	if (index.isValid())
		_treeView->edit(index);
}

void qtengine::ContentPanelProjectExplorer::onDeleteFile()
{
	auto index = _treeView->currentIndex();

	if (index.isValid()) {
		_fsModel->remove(index);
		_treeView->setCurrentIndex(QModelIndex());
		Manager::instance()->viewManager()->closeView();
	}
}
