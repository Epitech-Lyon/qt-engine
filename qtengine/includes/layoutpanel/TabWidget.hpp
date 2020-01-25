/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TabWidget
*/

#pragma once

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QMenu>

namespace qtengine {
	class ContentPanelBase;

	class TabWidget : public QTabWidget {
	public:
		TabWidget(QWidget * = nullptr);

		int tabAt(const QPoint &);

		ContentPanelBase *addTab(const QString &);
		ContentPanelBase *setTab(const QString &);
		ContentPanelBase *getPage(int) const;
		ContentPanelBase *closeTab(int);

	private:
		void split(Qt::Orientation);
		void initButton();
	};
}
