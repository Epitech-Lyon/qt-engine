/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsExport
*/

#pragma once

#include "DialogBase.hpp"

namespace qtengine {
	class DialogSettingsExport : public DialogBase {
	public:
		DialogSettingsExport(const QString &outputPath, QWidget *parent = nullptr);
		~DialogSettingsExport() = default;

		QString outputPath() const { return _outputPath; }
		bool generateMain() const { return _generateMain; }
		bool displayProgress() const { return _displayProgress; }

	private:
		QWidget *initBody();
		QString _outputPath;
		bool _generateMain;
		bool _displayProgress;
	};
}
