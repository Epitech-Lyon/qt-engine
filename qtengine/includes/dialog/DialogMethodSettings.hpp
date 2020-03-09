/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogMethodSettings
*/

#pragma once

#include "DialogBase.hpp"
#include "types/includes/Method.hpp"

namespace qtengine {
	class DialogMethodSettings : public DialogBase {
	public:
		DialogMethodSettings(const types::Method &method, QWidget *parent = nullptr);
		DialogMethodSettings(QWidget *parent = nullptr);
		~DialogMethodSettings() = default;

		types::Method method() const { return _method; }

	private:
		QWidget *initBody();
		types::Method _method;
	};
}
