/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** BoxTheme
*/

#pragma once

#include <QtWidgets/QDialog>

namespace qtengine {
	class BoxTheme : public QDialog {
	public:
		BoxTheme(QWidget * = nullptr);

	public:
		QString theme() const { return _theme; }

		void applyTheme(const QString &);

	private:
		void themeLight();
		void themeBlueDark();
		void themeOrangeDark();
		void themeDark();

		QString _theme;
	};
}
