/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogExport
*/

#pragma once

#include "DialogBase.hpp"

namespace qtengine {
	class DialogExport : public DialogBase {
	public:
		DialogExport(QWidget *parent = nullptr);
		~DialogExport();

	private:
		QWidget *initBody();
	};
}
