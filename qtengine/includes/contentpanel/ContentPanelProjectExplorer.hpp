/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelProjectExplorer
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtWidgets/QTreeView>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QFileSystemModel>

namespace qtengine {
	class ContentPanelProjectExplorer : public ContentPanelBase {
	public:
		ContentPanelProjectExplorer(QWidget * = nullptr);
		~ContentPanelProjectExplorer() = default;

		void init() override;

	private slots:
		void onModelIndexDoubleClicked(const QModelIndex &);
		void onCreateFile();
		void onRenameFile();
		void onDeleteFile();

	private:
		class TreeViewItemDelegate : public QStyledItemDelegate {
		public:
			TreeViewItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
			~TreeViewItemDelegate() = default;
			QString displayText(const QVariant &value, const QLocale &locale) const override;

		protected:
			void setEditorData(QWidget *editor, const QModelIndex &index) const override;
			void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

		private:
			QString _extension;
		};

		QToolBar *initToolBar() override;
		QTreeView *_treeView;
		QFileSystemModel *_fsModel;
	};
}
