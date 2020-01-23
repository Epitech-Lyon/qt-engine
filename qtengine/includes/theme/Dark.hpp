/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Dark
*/

#pragma once

#include <QApplication>
#include <QProxyStyle>

namespace qtengine {
	class Dark : public QProxyStyle {
	public:
		Dark();
		Dark(QStyle *style);

		QStyle *baseStyle() const;

		void polish(QApplication *) override;

	private:
		QStyle *styleBase(QStyle * = nullptr) const;
	};
}
