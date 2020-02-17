/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ESplitter
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QSplitter>

namespace libraryObjects {
	typedef Object<QSplitter> ESplitter;

	namespace Splitter {
		bool insertWidget(AObject *parent, int index, AObject *child);
		bool removeWidget(AObject *parent, AObject *child);
	}
}
