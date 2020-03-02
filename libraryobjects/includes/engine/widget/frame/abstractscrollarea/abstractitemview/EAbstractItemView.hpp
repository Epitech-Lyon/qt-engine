/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EAbstractItemView
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QAbstractItemView>

namespace libraryObjects {
	typedef Object<QAbstractItemView> EAbstractItemView;

	namespace AbstractItemView {
		bool setModel(AObject *parent, int, AObject *child);
		bool unsetModel(AObject *parent, AObject *child);
	}
}
