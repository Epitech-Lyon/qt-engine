/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** BlueDark
*/

#pragma once

#include <QApplication>
#include <QProxyStyle>

namespace qtengine {
	class BlueDark : public QProxyStyle {
	public:
		BlueDark();
		BlueDark(QStyle *style);

		QStyle *baseStyle() const;

		void polish(QPalette &) override;
		void polish(QApplication *) override;

	private:
		QStyle *styleBase(QStyle * = nullptr) const;
	};
}
