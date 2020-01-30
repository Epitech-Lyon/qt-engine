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
	QJsonArray jsonStateLayoutPanels;
	for (auto widget : widgets())
		jsonStateLayoutPanels.append(dynamic_cast<LayoutPanelBase*>(widget)->serialize());

	QJsonArray jsonStateSizes;
	for (auto size : _splitter->sizes())
		jsonStateSizes.append(size);

	QJsonObject jsonState;
	jsonState["Orientation"] = static_cast<int>(_splitter->orientation());
	jsonState["Sizes"] = jsonStateSizes;
	jsonState["LayoutPanels"] = jsonStateLayoutPanels;

	QJsonObject json;
	json["Type"] = "Splitter";
	json["State"] = jsonState;
	return json;
}

void qtengine::LayoutPanelSplitter::deserialize(const QJsonObject &jsonState)
{
	for (auto widget : widgets())
		delete widget;

	_splitter->setOrientation(static_cast<Qt::Orientation>(jsonState["Orientation"].toInt()));
	for (auto jsonStateLayoutPanelRef : jsonState["LayoutPanels"].toArray()) {
		auto jsonStateLayoutPanel = jsonStateLayoutPanelRef.toObject();
		auto type = jsonStateLayoutPanel["Type"].toString();
		LayoutPanelBase *base = nullptr;
		
		if (type == "Tabber")
			base = new LayoutPanelTabber(this);
		else if (type == "Splitter")
			base = new LayoutPanelSplitter(Qt::Horizontal, this);
		base->deserialize(jsonStateLayoutPanel["State"].toObject());
		_splitter->addWidget(base);
	}

	QList<int> sizes;
	for (auto sizeRef : jsonState["Sizes"].toArray())
		sizes << sizeRef.toInt();
	_splitter->setSizes(sizes);
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
