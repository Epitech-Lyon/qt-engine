/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsMethod
*/

#pragma once

#include "DialogBase.hpp"
#include "types/includes/Method.hpp"

namespace qtengine {
	class DialogSettingsMethod : public DialogBase {
	public:
		DialogSettingsMethod(const types::Method &method, const QString &windowTitle, QWidget *parent = nullptr);
		DialogSettingsMethod(const QString &windowTitle, QWidget *parent = nullptr);
		~DialogSettingsMethod() = default;

		types::Method method() const { return _method; }

	private:
		QWidget *initBody();
		void addParameter(QMetaType::Type parameterType, const QString &parameterName);
		types::Method _method;
		QVBoxLayout *_layoutParameters;
	};
}
