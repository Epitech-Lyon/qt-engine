/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsClassType
*/

#pragma once

#include "DialogBase.hpp"

namespace types {
	class ClassType;
}

namespace qtengine {
	class DialogSettingsClassType : public DialogBase {
	public:
		DialogSettingsClassType(types::ClassType *classType, QWidget *parent = nullptr);
		~DialogSettingsClassType() = default;
	};
}
