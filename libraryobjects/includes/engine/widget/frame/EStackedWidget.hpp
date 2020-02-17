/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EStackedWidget
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QStackedWidget>

namespace libraryObjects {
	typedef Object<QStackedWidget> EStackedWidget;

	namespace StackedWidget {
		bool insertWidget(AObject *parent, int index, AObject *child);
		bool removeWidget(AObject *parent, AObject *child);
	}
}
