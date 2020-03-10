/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsProperty
*/

#pragma once

#include "DialogBase.hpp"
#include "types/includes/Property.hpp"

namespace qtengine {
	class DialogSettingsProperty : public DialogBase {
	public:
		DialogSettingsProperty(const types::Property &property, QWidget *parent = nullptr);
		DialogSettingsProperty(QWidget *parent = nullptr);
		~DialogSettingsProperty() = default;

		types::Property property() const { return _property; }

	private:
		QWidget *initBody();
		types::Property _property;
	};
}
