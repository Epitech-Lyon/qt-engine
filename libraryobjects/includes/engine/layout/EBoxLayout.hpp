/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EBoxLayout
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QBoxLayout>

namespace libraryObjects {
	typedef Object<QBoxLayout> EBoxLayout;

	AObject *addWidget(AObject *parent, int index, LibraryObject *child);
}
