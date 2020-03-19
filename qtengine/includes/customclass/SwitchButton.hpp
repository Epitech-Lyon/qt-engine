/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** SwitchButton
*/

#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtCore/QPropertyAnimation>

namespace qtengine {
	class SwitchButton : public QWidget
	{
		Q_OBJECT
		Q_DISABLE_COPY(SwitchButton)

	public:
		explicit SwitchButton(const QString &firstText, const QString &secondText, QWidget* parent = nullptr);
		~SwitchButton();

		//-- QWidget methods
		void setEnabled(bool enable);

		//-- Setters
		void setDuration(int duration);
		void setValue(bool value);

		//-- Getters
		bool value() const;

	signals:
		void valueChanged(bool newvalue);

	private:
		class SwitchCircle;
		class SwitchBackground;

		void mousePressEvent(QMouseEvent *event) override;
		void paintEvent(QPaintEvent* event) override;
		void update();

	private:
		bool _value;
		int  _duration;

		QLinearGradient _lg;
		QLinearGradient _lg2;
		QLinearGradient _lg_disabled;

		QColor _pencolor;
		QColor _offcolor;
		QColor _oncolor;
		int    _tol;
		int    _borderradius;

		// This order for definition is important (these widgets overlap)
		QLabel*           _labeloff;
		SwitchBackground* _background;
		QLabel*           _labelon;
		SwitchCircle*     _circle;

		bool _enabled;

		QPropertyAnimation* __btn_move;
		QPropertyAnimation* __back_move;
	};

	class SwitchButton::SwitchBackground : public QWidget
	{
		Q_OBJECT
		Q_DISABLE_COPY(SwitchBackground)

	public:
		explicit SwitchBackground(QWidget* parent = nullptr, QColor color = QColor(154, 205, 50), bool rect = false);
		~SwitchBackground() = default;

		//-- QWidget methods
		void paintEvent(QPaintEvent* event) override;
		void setEnabled(bool);

	private:
		bool            _rect;
		int             _borderradius;
		QColor          _color;
		QColor          _pencolor;
		QLinearGradient _lg;
		QLinearGradient _lg_disabled;

		bool _enabled;
	};


	class SwitchButton::SwitchCircle : public QWidget
	{
		Q_OBJECT
		Q_DISABLE_COPY(SwitchCircle)

	public:
		explicit SwitchCircle(QWidget* parent = nullptr, QColor color = QColor(255, 255, 255), bool rect = false);
		~SwitchCircle() = default;

		//-- QWidget methods
		void paintEvent(QPaintEvent* event) override;
		void setEnabled(bool);

	private:
		bool            _rect;
		int             _borderradius;
		QColor          _color;
		QColor          _pencolor;
		QRadialGradient _rg;
		QLinearGradient _lg;
		QLinearGradient _lg_disabled;

		bool _enabled;
	};
}
