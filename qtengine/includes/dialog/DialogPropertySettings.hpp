/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogPropertySettings
*/

#pragma once

#include "DialogBase.hpp"
#include "types/includes/Property.hpp"

namespace qtengine {
	class DialogPropertySettings : public DialogBase {
	public:
		DialogPropertySettings(const types::Property &property, QWidget *parent = nullptr);
		DialogPropertySettings(QWidget *parent = nullptr);
		~DialogPropertySettings() = default;

		types::Property property() const { return _property; }

	private:
		QWidget *initBody();
		types::Property _property;
	};
}
