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

	AObject *setLayout(AObject *parent, int index, LibraryObject *child);
}
