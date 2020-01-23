/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelSplitter
*/

#include "LayoutPanelSplitter.hpp"
#include "LayoutPanelTabber.hpp"

qtengine::LayoutPanelSplitter::LayoutPanelSplitter(QWidget *parent)
	: LayoutPanelBase(parent)
	, _splitter(new QSplitter(this))
	, _firstLayoutPanel(nullptr)
	, _secondLayoutPanel(nullptr)
{
	_splitter->setHandleWidth(4);
	_mainLayout->addWidget(_splitter);
}

QJsonObject qtengine::LayoutPanelSplitter::serialize() const
{
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
}

void qtengine::LayoutPanelSplitter::deserialize(const QJsonObject &jsonState)
{
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
}

void qtengine::LayoutPanelSplitter::setChildrenPanel(LayoutPanelBase *firstLayoutPanel, LayoutPanelBase *secondLayoutPanel)
{
	removeAllPanels();
	_firstLayoutPanel = firstLayoutPanel;
	_secondLayoutPanel = secondLayoutPanel;
	_splitter->addWidget(_firstLayoutPanel);
	_splitter->addWidget(_secondLayoutPanel);
	_splitter->setSizes(QList<int>({ INT_MAX, INT_MAX }));
}

void qtengine::LayoutPanelSplitter::setChildPanel(int index, LayoutPanelBase *layoutPanel)
{
	if (index == 0)
		setFirstLayoutPanel(layoutPanel);
	else if (index == 1)
		setSecondLayoutPanel(layoutPanel);
}

void qtengine::LayoutPanelSplitter::setFirstLayoutPanel(LayoutPanelBase *layoutPanel)
{
	_firstLayoutPanel = layoutPanel;
	_firstLayoutPanel->setParent(this);
	_splitter->replaceWidget(0, _firstLayoutPanel);
}

void qtengine::LayoutPanelSplitter::setSecondLayoutPanel(LayoutPanelBase *layoutPanel)
{
	_secondLayoutPanel = layoutPanel;
	_secondLayoutPanel->setParent(this);
	_splitter->replaceWidget(1, _secondLayoutPanel);
}

void qtengine::LayoutPanelSplitter::setOrientation(Qt::Orientation orientation)
{
	_splitter->setOrientation(orientation);
}

int qtengine::LayoutPanelSplitter::indexOf(LayoutPanelBase *layoutPanel)
{
	return _splitter->indexOf(layoutPanel);
}

void qtengine::LayoutPanelSplitter::removeAllPanels()
{
	for (int i = 0; i < _splitter->count(); i++)
		_splitter->widget(i)->setParent(nullptr);
}
