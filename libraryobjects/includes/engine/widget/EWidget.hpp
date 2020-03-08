/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EWidget
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QWidget>

namespace libraryObjects {
	typedef Object<QWidget> EWidget;

	class Widget : public QWidget {
		Q_OBJECT

	public:
		static bool setLayout(AObject *parent, int index, AObject *child);
		static bool unsetLayout(AObject *parent, AObject *child);
	};
}
