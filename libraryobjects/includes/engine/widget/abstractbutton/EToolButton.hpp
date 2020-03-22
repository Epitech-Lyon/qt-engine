/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EToolButton
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QToolButton>

namespace libraryObjects {
	typedef Object<QToolButton> EToolButton;

	namespace ToolButton {
		bool setMenu(AObject *parent, int index, AObject *child);
		bool unsetMenu(AObject *parent, AObject *child);
	}
}
