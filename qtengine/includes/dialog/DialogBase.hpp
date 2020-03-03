/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogBase
*/

#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtCore/QList>

namespace qtengine {
	class DialogBase : public QDialog {
	public:
		DialogBase(QWidget * = nullptr);
		~DialogBase() = default;

	protected:
		void addWidgetTo(QWidget *widgetToAdd, const QString &name, QLayout *layoutParent);
		QVBoxLayout *_mainLayout;
		QDialogButtonBox *_buttonBox;

	private:
		void resizeWidgets();
		QList<QLabel *> _labels;
	};
}
