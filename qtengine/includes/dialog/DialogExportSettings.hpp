/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogExportSettings
*/

#pragma once

#include "DialogBase.hpp"

namespace qtengine {
	class DialogExportSettings : public DialogBase {
	public:
		DialogExportSettings(const QString &outputPath, QWidget *parent = nullptr);
		~DialogExportSettings();

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
