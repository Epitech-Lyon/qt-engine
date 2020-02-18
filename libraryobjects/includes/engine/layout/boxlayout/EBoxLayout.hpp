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

	namespace BoxLayout {
		bool insertWidget(AObject *parent, int index, AObject *child);

		bool insertLayout(AObject *parent, int index, AObject *child);
	}
}
