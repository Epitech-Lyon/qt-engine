/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EComboBox
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QComboBox>

namespace libraryObjects {
	typedef Object<QComboBox> EComboBox;

	namespace ComboBox {
		void editItems(AObject *object);
	}
}
