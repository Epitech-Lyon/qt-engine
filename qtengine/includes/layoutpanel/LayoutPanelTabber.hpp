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

		int addView(ContentPanelBase *);
		int insertView(const QPoint &, ContentPanelBase *);
		void removeView(int index);

		void setCurrentView(int index);
		QWidget *currentView();

		TabWidget *tabWidget() { return _tabWidget; }

	private:
		TabWidget *_tabWidget;
	};
}
