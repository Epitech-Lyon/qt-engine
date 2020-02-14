/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewExplorer
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtWidgets/QTreeWidget>

namespace libraryObjects {
	class AObject;	
}

namespace qtengine {
	class ContentPanelViewExplorer : public ContentPanelBase {
	public:
		ContentPanelViewExplorer(QWidget * = nullptr);
		~ContentPanelViewExplorer() = default;

		void init() override;

	private:
		QToolBar *initToolBar() override;
		void buildTree(QTreeWidgetItem *, libraryObjects::AObject *);
		QTreeWidget *_treeWidget;
	};
};
