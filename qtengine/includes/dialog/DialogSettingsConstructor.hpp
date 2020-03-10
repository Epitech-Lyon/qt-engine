/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsConstructor
*/

#pragma once

#include "DialogBase.hpp"
#include "types/includes/Constructor.hpp"

namespace qtengine {
	class DialogSettingsConstructor : public DialogBase {
	public:
		DialogSettingsConstructor(const types::Constructor &constructor, QWidget *parent = nullptr);
		DialogSettingsConstructor(QWidget *parent = nullptr);
		~DialogSettingsConstructor() = default;

		types::Constructor constructor() const { return _constructor; }

	private:
		QWidget *initBody();
		void addParameter(QMetaType::Type parameterType, const QString &parameterName);
		types::Constructor _constructor;
		QVBoxLayout *_layoutParameters;
	};
}
