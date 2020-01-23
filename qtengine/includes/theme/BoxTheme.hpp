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

	private:
		void applyTheme(const QString &);

		void themeLight();
		void themeBlueDark();
		void themeOrangeDark();
		void themeDark();
	};
}
