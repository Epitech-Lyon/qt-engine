/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EScrollArea
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QScrollArea>

namespace libraryObjects {
	typedef Object<QScrollArea> EScrollArea;

	bool setWidget(AObject *parent, int, AObject *child);
	bool unsetWidget(AObject *parent, AObject *child);
}
