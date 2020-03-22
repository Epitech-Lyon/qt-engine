/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EMenuBar
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QMenuBar>

namespace libraryObjects {
	typedef Object<QMenuBar> EMenuBar;

	namespace MenuBar {
		bool insertAction(AObject *parent, int index, AObject *child);
		bool removeAction(AObject *parent, AObject *child);

		bool insertMenu(AObject *parent, int index, AObject *child);
		bool removeMenu(AObject *parent, AObject *child);
	}
}
