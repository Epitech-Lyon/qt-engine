/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EToolBox
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QToolBox>

namespace libraryObjects {
	typedef Object<QToolBox> EToolBox;

	namespace ToolBox {
		bool insertItem(AObject *parent, int index, AObject *child);
		bool removeItem(AObject *parent, AObject *child);

		void setText(AObject *object);
		void setToolTip(AObject *object);
		void setIcon(AObject *object);
		void setEnable(AObject *object);
	}
}
