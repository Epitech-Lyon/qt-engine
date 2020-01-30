/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewExplorer
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtWidgets/QTreeWidget>

namespace qtengine {
	class ContentPanelViewExplorer : public ContentPanelBase {
	public:
		ContentPanelViewExplorer(QWidget * = nullptr);
		~ContentPanelViewExplorer() = default;

		void init() override;

	private:
		QToolBar *initToolBar() override;
		void buildTree(QTreeWidgetItem *, const QJsonObject &);
		QTreeWidget *_treeWidget;
	};
};
