/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ELayout
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QLayout>

namespace libraryObjects {
	typedef Object<QLayout> ELayout;

	namespace Layout {
		bool removeWidget(AObject *parent, AObject *child);

		bool removeLayout(AObject *parent, AObject *child);
	}
}
