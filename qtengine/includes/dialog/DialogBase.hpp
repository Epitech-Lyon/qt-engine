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
		DialogBase(const QString &windowTitle, QWidget *parent = nullptr);
		~DialogBase() = default;

	protected:
		int labelsWidth() const { return _labelsWidth; }
		QWidget *addWidgetTo(QWidget *widgetToAdd, const QString &name, QBoxLayout *layoutParent);
		QWidget *insertWidgetTo(int index, QWidget *widgetToAdd, const QString &name, QBoxLayout *layoutParent);
		QVBoxLayout *_mainLayout;
		QDialogButtonBox *_buttonBox;

	private:
		void resizeWidgets();
		QList<QLabel *> _labels;
		int _labelsWidth;
	};
}
