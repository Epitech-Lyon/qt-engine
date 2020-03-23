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
		DialogSettingsExport(const QString &outputPath, const QString &generateMainFrom, bool displayProgress, QWidget *parent = nullptr);
		~DialogSettingsExport() = default;

		QString outputPath() const { return _outputPath; }
		QString generateMainFrom() const { return _generateMainFrom; }
		bool displayProgress() const { return _displayProgress; }

	private:
		QWidget *initBody();
		QString _outputPath;
		QString _generateMainFrom;
		bool _displayProgress;
	};
}
