/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** SwitchButton
*/

#include "moc_SwitchButton.cpp"
#include "SwitchButton.hpp"

#include <QtGui/QPainter>

qtengine::SwitchButton::SwitchButton(const QString &firstText, const QString &secondText, QWidget* parent)
	: QWidget(parent)
	, _value(false)
	, _duration(100)
	, _enabled(true)
{
	_pencolor = QColor(120, 120, 120);

	_lg = QLinearGradient(35, 30, 35, 0);
	_lg.setColorAt(0, QColor(210, 210, 210));
	_lg.setColorAt(0.25, QColor(255, 255, 255));
	_lg.setColorAt(0.82, QColor(255, 255, 255));
	_lg.setColorAt(1, QColor(210, 210, 210));

	_lg2 = QLinearGradient(50, 30, 35, 0);
	_lg2.setColorAt(0, QColor(230, 230, 230));
	_lg2.setColorAt(0.25, QColor(255, 255, 255));
	_lg2.setColorAt(0.82, QColor(255, 255, 255));
	_lg2.setColorAt(1, QColor(230, 230, 230));

	_lg_disabled = QLinearGradient(50, 30, 35, 0);
	_lg_disabled.setColorAt(0, QColor(200, 200, 200));
	_lg_disabled.setColorAt(0.25, QColor(230, 230, 230));
	_lg_disabled.setColorAt(0.82, QColor(230, 230, 230));
	_lg_disabled.setColorAt(1, QColor(200, 200, 200));

	_offcolor = QColor(255, 255, 255);
	_oncolor = QColor(154, 205, 50);
	_tol = 0;
	_borderradius = 12;
	_labeloff = new QLabel(this);
	_background = new SwitchBackground(this, _oncolor);
	_labelon = new QLabel(this);
	_circle = new SwitchCircle(this, _offcolor);
	__btn_move = new QPropertyAnimation(this);
	__back_move = new QPropertyAnimation(this);

	__btn_move->setTargetObject(_circle);
	__btn_move->setPropertyName("pos");
	__back_move->setTargetObject(_background);
	__back_move->setPropertyName("size");

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	_labeloff->setText(firstText);
	_labeloff->setFixedSize(_labeloff->minimumSizeHint() + QSize(5, 0));
	_labelon->setText(secondText);
	_labelon->setFixedSize(_labelon->minimumSizeHint() + QSize(5, 0));

	int maxSize = 60;
	maxSize = _labeloff->width() > maxSize ? _labeloff->width() : maxSize;
	maxSize = _labelon->width() > maxSize ? _labelon->width() : maxSize;
	maxSize += _circle->width() + 20;

	_labeloff->move(maxSize - _labeloff->width() - 10, 5);
	_labelon->move(10, 5);
	setFixedSize(QSize(maxSize, 24));
	
	_labeloff->setStyleSheet("color: rgb(120, 120, 120); font-weight: bold;");
	_labelon->setStyleSheet("color: rgb(255, 255, 255); font-weight: bold;");

	_background->resize(20, 20);

	_background->move(2, 2);
	_circle->move(2, 2);
}

qtengine::SwitchButton::~SwitchButton()
{
	delete _circle;
	delete _background;
	delete _labeloff;
	delete _labelon;
	delete __btn_move;
	delete __back_move;
}

void qtengine::SwitchButton::paintEvent(QPaintEvent *)
{
	QPainter* painter = new QPainter;
	painter->begin(this);
	painter->setRenderHint(QPainter::Antialiasing, true);

	QPen pen(Qt::NoPen);
	painter->setPen(pen);

	painter->setBrush(_pencolor);
	painter->drawRoundedRect(0, 0
		, width(), height()
		, 12, 12);

	painter->setBrush(_lg);
	painter->drawRoundedRect(1, 1
		, width() - 2, height() - 2
		, 10, 10);

	painter->setBrush(QColor(210, 210, 210));
	painter->drawRoundedRect(2, 2
		, width() - 4, height() - 4
		, 10, 10);

	if (_enabled)
	{
		painter->setBrush(_lg2);
		painter->drawRoundedRect(3, 3
			, width() - 6, height() - 6
			, 7, 7);
	}
	else
	{
		painter->setBrush(_lg_disabled);
		painter->drawRoundedRect(3, 3
			, width() - 6, height() - 6
			, 7, 7);
	}
	painter->end();
}

void qtengine::SwitchButton::mousePressEvent(QMouseEvent *)
{
	if (!_enabled) { return; }

	__btn_move->stop();
	__back_move->stop();

	__btn_move->setDuration(_duration);
	__back_move->setDuration(_duration);

	int hback = 20;
	QSize initial_size(hback, hback);
	QSize final_size(width() - 4, hback);

	int xi = 2;
	int y  = 2;
	int xf = width() - 22;

	if (_value)
	{
		final_size = QSize(hback, hback);
		initial_size = QSize(width() - 4, hback);

		xi = xf;
		xf = 2;
	}

	__btn_move->setStartValue(QPoint(xi, y));
	__btn_move->setEndValue(QPoint(xf, y));

	__back_move->setStartValue(initial_size);
	__back_move->setEndValue(final_size);

	__btn_move->start();
	__back_move->start();

	// Assigning new current value
	_value = !_value;
	emit valueChanged(_value);
}

void qtengine::SwitchButton::setEnabled(bool enable)
{
	_enabled = enable;
	_circle->setEnabled(_enabled);
	_background->setEnabled(_enabled);
	if (_enabled || value())
		_labelon->show();
	else
		_labelon->hide();
	QWidget::setEnabled(_enabled);
}

void qtengine::SwitchButton::setDuration(int duration)
{
	_duration = duration;
}

void qtengine::SwitchButton::setValue(bool value)
{
	if (value == _value) { return; }

	auto saveDuration = _duration;

	setDuration(0);
	mousePressEvent(nullptr);
	setDuration(saveDuration);
}

bool qtengine::SwitchButton::value() const
{
	return _value;
}

void qtengine::SwitchButton::update()
{
	int hback = 20;
	QSize final_size(width() - 4, hback);

	int y = 2;
	int xf = width() - 22;

	if (_value)
	{
		final_size = QSize(hback, hback);
		xf = 2;
	}

	_circle->move(QPoint(xf, y));
	_background->resize(final_size);
}

qtengine::SwitchButton::SwitchBackground::SwitchBackground(QWidget* parent, QColor color, bool rect)
	: QWidget(parent)
	, _rect(rect)
	, _borderradius(12)
	, _color(color)
	, _pencolor(QColor(170, 170, 170))
{
	setFixedHeight(20);

	_lg = QLinearGradient(0, 25, 70, 0);
	_lg.setColorAt(0, QColor(154, 194, 50));
	_lg.setColorAt(0.25, QColor(154, 210, 50));
	_lg.setColorAt(0.95, QColor(154, 194, 50));

	_lg_disabled = QLinearGradient(0, 25, 70, 0);
	_lg_disabled.setColorAt(0, QColor(190, 190, 190));
	_lg_disabled.setColorAt(0.25, QColor(230, 230, 230));
	_lg_disabled.setColorAt(0.95, QColor(190, 190, 190));

	if (_rect)
		_borderradius = 0;

	_enabled = true;
}

void qtengine::SwitchButton::SwitchBackground::paintEvent(QPaintEvent*)
{
	QPainter* painter = new QPainter;
	painter->begin(this);
	painter->setRenderHint(QPainter::Antialiasing, true);

	QPen pen(Qt::NoPen);
	painter->setPen(pen);
	if (_enabled)
	{
		painter->setBrush(QColor(154, 190, 50));
		painter->drawRoundedRect(0, 0
			, width(), height()
			, 10, 10);

		painter->setBrush(_lg);
		painter->drawRoundedRect(1, 1, width()-2, height()-2, 8, 8);
	}
	else
	{
		painter->setBrush(QColor(150, 150, 150));
		painter->drawRoundedRect(0, 0
			, width(), height()
			, 10, 10);

		painter->setBrush(_lg_disabled);
		painter->drawRoundedRect(1, 1, width() - 2, height() - 2, 8, 8);
	}
	painter->end();
}

void qtengine::SwitchButton::SwitchBackground::setEnabled(bool flag)
{
	_enabled = flag;
}

qtengine::SwitchButton::SwitchCircle::SwitchCircle(QWidget* parent, QColor color, bool rect)
	: QWidget(parent)
	, _rect(rect)
	, _borderradius(12)
	, _color(color)
	, _pencolor(QColor(120, 120, 120))
{
	setFixedSize(20, 20);

	_rg = QRadialGradient(static_cast<int>(width() / 2), static_cast<int>(height() / 2), 12);
	_rg.setColorAt(0, QColor(255, 255, 255));
	_rg.setColorAt(0.6, QColor(255, 255, 255));
	_rg.setColorAt(1, QColor(205, 205, 205));

	_lg = QLinearGradient(3, 18, 20, 4);
	_lg.setColorAt(0, QColor(255, 255, 255));
	_lg.setColorAt(0.55, QColor(230, 230, 230));
	_lg.setColorAt(0.72, QColor(255, 255, 255));
	_lg.setColorAt(1, QColor(255, 255, 255));

	_lg_disabled = QLinearGradient(3, 18, 20, 4);
	_lg_disabled.setColorAt(0, QColor(230, 230, 230));
	_lg_disabled.setColorAt(0.55, QColor(210, 210, 210));
	_lg_disabled.setColorAt(0.72, QColor(230, 230, 230));
	_lg_disabled.setColorAt(1, QColor(230, 230, 230));

	_enabled = true;
}

void qtengine::SwitchButton::SwitchCircle::paintEvent(QPaintEvent*)
{
	QPainter* painter = new QPainter;
	painter->begin(this);
	painter->setRenderHint(QPainter::Antialiasing, true);

	QPen pen(Qt::NoPen);
	painter->setPen(pen);
	painter->setBrush(_pencolor);

	painter->drawEllipse(0, 0, 20, 20);
	painter->setBrush(_rg);
	painter->drawEllipse(1, 1, 18, 18);

	painter->setBrush(QColor(210, 210, 210));
	painter->drawEllipse(2, 2, 16, 16);

	if (_enabled)
	{
		painter->setBrush(_lg);
		painter->drawEllipse(3, 3, 14, 14);
	}
	else
	{
		painter->setBrush(_lg_disabled);
		painter->drawEllipse(3, 3, 14, 14);
	}

	painter->end();
}

void qtengine::SwitchButton::SwitchCircle::setEnabled(bool flag)
{
	_enabled = flag;
}
