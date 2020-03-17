/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelProjectExplorer
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtWidgets/QTreeView>

namespace qtengine {
	class TreeWidgetProjectExplorer;

	class ContentPanelProjectExplorer : public ContentPanelBase {
	public:
		ContentPanelProjectExplorer(QWidget * = nullptr);
		~ContentPanelProjectExplorer() = default;

		void init() override;

	private slots:
		void onViewsChanged(const QStringList &viewsPath);
		void onRenameFile();

	private:
		QToolBar *initToolBar() override;
		TreeWidgetProjectExplorer *_tree;
	};
}
