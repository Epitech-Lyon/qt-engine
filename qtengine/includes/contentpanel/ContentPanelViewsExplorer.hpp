/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewsExplorer
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtWidgets/QTreeWidget>

namespace qtengine {
	class ContentPanelViewsExplorer : public ContentPanelBase {
	public:
		ContentPanelViewsExplorer(QWidget * = nullptr);
		~ContentPanelViewsExplorer() = default;

		void init() override;

	private:
		QToolBar *initToolBar() override;
		QTreeWidget *_treeWidget;
	};
};
