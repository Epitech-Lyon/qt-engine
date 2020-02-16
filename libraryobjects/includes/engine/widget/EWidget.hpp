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

	bool setLayout(AObject *parent, int index, AObject *child);
	bool unsetLayout(AObject *parent, AObject *child);
}
