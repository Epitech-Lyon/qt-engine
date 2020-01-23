/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TabGhost
*/

#pragma once

#include <QtWidgets/QWidget>

namespace qtengine {
	class TabBar;
	class TabWidget;

	class TabGhost : public QWidget {
	public:
		TabGhost(const QPoint &, TabBar *);

		int index() const { return _index; }
		void moveWithOffset(const QPoint &);
		bool dragStarted(const QPoint &);

	private:
		TabWidget *createTabGhost(TabBar *);
		int getWidth(TabBar *);
		int _index;
		QPoint _offset;
		QPoint _originPos;
		QRect _geometry;
	};

};
