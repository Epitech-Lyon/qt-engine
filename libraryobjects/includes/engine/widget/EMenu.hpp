/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EMenu
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QMenu>

namespace libraryObjects {
	typedef Object<QMenu> EMenu;

	namespace Menu {
		bool insertAction(AObject *parent, int index, AObject *child);
		bool removeAction(AObject *parent, AObject *child);

		bool insertMenu(AObject *parent, int index, AObject *child);
		bool removeMenu(AObject *parent, AObject *child);
	}
}
