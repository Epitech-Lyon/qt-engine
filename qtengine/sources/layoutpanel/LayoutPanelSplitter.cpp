/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelSplitter
*/

#include "LayoutPanelSplitter.hpp"
#include "LayoutPanelTabber.hpp"

qtengine::LayoutPanelSplitter::LayoutPanelSplitter(LayoutPanelTabber *tabber, Qt::Orientation orientation, QWidget *parent)
	: LayoutPanelBase(parent)
	, _splitter(new QSplitter(orientation, this))
{
	_splitter->addWidget(tabber);
	_splitter->addWidget(new LayoutPanelTabber(this));
	_splitter->setHandleWidth(4);
	_mainLayout->addWidget(_splitter);
}

qtengine::LayoutPanelSplitter::LayoutPanelSplitter(Qt::Orientation orientation, QWidget *parent)
	: LayoutPanelBase(parent)
	, _splitter(new QSplitter(orientation, this))
{
	_splitter->addWidget(new LayoutPanelTabber(this));
	_splitter->setHandleWidth(4);
	_mainLayout->addWidget(_splitter);
}

QJsonObject qtengine::LayoutPanelSplitter::serialize() const
{
	/*
	QList<int> size = _splitter->sizes();
	double handlePos = ((double)size[0]) / ((double)(size[0] + size[1]));
	QString orientation = _splitter->orientation() == Qt::Orientation::Horizontal ? "Horizontal" : "Vertical";

	QJsonObject jsonState;
	jsonState["HandlePos"] = handlePos;
	jsonState["Orientation"] = orientation;
	jsonState["FirstLayoutPanel"] = _firstLayoutPanel->serialize();
	jsonState["SecondLayoutPanel"] = _secondLayoutPanel->serialize();

	QJsonObject splitterObj;
	splitterObj["Type"] = "Splitter";
	splitterObj["State"] = jsonState;
	return splitterObj;
	*/
	return QJsonObject();
}

void qtengine::LayoutPanelSplitter::deserialize(const QJsonObject &)
{
	/*
	{
		QJsonObject firstLayoutPanelObject = jsonState["FirstLayoutPanel"].toObject();
		QString type = firstLayoutPanelObject["Type"].toString();
		if (type == "Splitter")
			_firstLayoutPanel = new LayoutPanelSplitter(_splitter);
		else if (type == "Tabber")
			_firstLayoutPanel = new LayoutPanelTabber(_splitter);
		//setUpLeftLayoutPanel(_firstLayoutPanel);
		_firstLayoutPanel->deserialize(firstLayoutPanelObject["State"].toObject());
	}
	{
		QJsonObject secondLayoutPanelObject = jsonState["SecondLayoutPanel"].toObject();
		QString type = secondLayoutPanelObject["Type"].toString();
		if (type == "Splitter")
			_secondLayoutPanel = new LayoutPanelSplitter(_splitter);
		else if (type == "Tabber")
			_secondLayoutPanel = new LayoutPanelTabber(_splitter);
		//setUpLeftLayoutPanel(_secondLayoutPanel);
		_secondLayoutPanel->deserialize(secondLayoutPanelObject["State"].toObject());
	}

	setChildrenPanel(_firstLayoutPanel, _secondLayoutPanel);

	QString orientation = jsonState["Orientation"].toString();
	if (orientation == "Vertical")
		_splitter->setOrientation(Qt::Vertical);
	else if (orientation == "Horizontal")
		_splitter->setOrientation(Qt::Horizontal);

	double handlePos = jsonState["HandlePos"].toDouble();
	_splitter->setSizes({ (int)(handlePos * INT_MAX), (int)((1.0 - handlePos) * INT_MAX) });
	*/
}

void qtengine::LayoutPanelSplitter::removeTabber(LayoutPanelTabber *tabber)
{
	tabber->deleteLater();
	if (_splitter->count() > 2) { return; }
	auto base = parentLayoutPanel();
	auto splitter = dynamic_cast<LayoutPanelSplitter*>(base);

	if (splitter)
		splitter->unsplit(tabber, this);
	else if (base) {
		LayoutPanelBase *saved;

		for (auto widget : widgets())
			if (widget != tabber)
				saved = dynamic_cast<LayoutPanelBase *>(widget);
		base->setChild(saved);
		deleteLater();
	}	
}

void qtengine::LayoutPanelSplitter::split(Qt::Orientation orientation, LayoutPanelTabber *tabber)
{
	if (orientation == _splitter->orientation())
		_splitter->addWidget(new LayoutPanelTabber(this));
	else {
		auto index = _splitter->indexOf(tabber);

		_splitter->insertWidget(index, new LayoutPanelSplitter(tabber, orientation, this));
	}
}

void qtengine::LayoutPanelSplitter::unsplit(LayoutPanelBase *deleted, LayoutPanelSplitter *splitter)
{
	LayoutPanelBase *saved;

	for (auto widget : splitter->widgets())
		if (widget != deleted)
			saved = dynamic_cast<LayoutPanelBase *>(widget);
	_splitter->replaceWidget(_splitter->indexOf(splitter), saved);
	splitter->deleteLater();

	auto isSplitter = dynamic_cast<LayoutPanelSplitter*>(saved);
	if (isSplitter && isSplitter->_splitter->orientation() == _splitter->orientation()) {
		auto index = _splitter->indexOf(isSplitter);
		auto widgets = isSplitter->widgets();

		for (int i = 0; i < widgets.size(); i += 1)
			_splitter->insertWidget(index + i, widgets[i]);
		isSplitter->deleteLater();
	}
}

QVector<QWidget *> qtengine::LayoutPanelSplitter::widgets() const
{
	QVector<QWidget *> ret;

	for (int i = 0; i < _splitter->count(); i += 1)
		ret << _splitter->widget(i);
	return ret;
}
