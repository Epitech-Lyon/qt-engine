/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TabBar
*/

#pragma once

#include <QtWidgets/QTabBar>

namespace qtengine {
	class TabGhost;
	class LayoutPanelTabber;

	class TabBar : public QTabBar {
	public:
		TabBar(QWidget * = nullptr);
		~TabBar() = default;

	private:
		void mousePressEvent(QMouseEvent *) override;
		void mouseMoveEvent(QMouseEvent *) override;
		void mouseReleaseEvent(QMouseEvent *) override;

		void moveToWindow(LayoutPanelTabber *, const QPoint &);

		TabGhost *_tabGhost;
		QPoint _mousePressedPos;
		bool _isDragging;
	};
}
