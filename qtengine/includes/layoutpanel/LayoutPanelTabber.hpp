/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelTabber
*/

#pragma once

#include "LayoutPanelBase.hpp"

namespace qtengine {
	class ContentPanelBase;
	class TabWidget;

	class LayoutPanelTabber : public LayoutPanelBase {
	public:
		LayoutPanelTabber(QWidget * = nullptr);
		~LayoutPanelTabber() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

		int addTab(ContentPanelBase *);
		int insertTab(const QPoint &, ContentPanelBase *);
		void closeTab(int index);

		void setCurrentTab(int index);
		QWidget *currentTab();

		TabWidget *tabWidget() { return _tabWidget; }

	private:
		TabWidget *_tabWidget;
	};
}
