/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelFileExplorer
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtWidgets/QTreeView>
#include <QtWidgets/QFileSystemModel>

namespace qtengine {
	class ContentPanelFileExplorer : public ContentPanelBase {
		public:
			ContentPanelFileExplorer(QWidget * = nullptr);
			~ContentPanelFileExplorer() = default;

			void init() override;

		private slots:
			void onModelIndexClicked(const QModelIndex &);
			void onCreateFile();
			void onRenameFile();
			void onDeleteFile();

		private:
			QToolBar *initToolBar() override;
			QTreeView *_treeView;
			QFileSystemModel *_fsModel;
	};
}
