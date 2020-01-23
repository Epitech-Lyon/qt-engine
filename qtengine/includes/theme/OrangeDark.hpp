/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** OrangeDark
*/

#pragma once

#include <QApplication>
#include <QProxyStyle>

namespace qtengine {
	class OrangeDark : public QProxyStyle {
	public:
		OrangeDark();
		OrangeDark(QStyle *style);

		QStyle *baseStyle() const;

		void polish(QApplication *) override;

	private:
		QStyle *styleBase(QStyle * = nullptr) const;
	};
}
