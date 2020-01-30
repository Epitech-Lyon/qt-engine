/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelTabber
*/

#include "LayoutPanelTabber.hpp"

#include "LayoutPanelSplitter.hpp"
#include "ContentPanelBase.hpp"
#include "ContentPanelFactory.hpp"
#include "TabWidget.hpp"
#include <QtCore/QJsonArray>

qtengine::LayoutPanelTabber::LayoutPanelTabber(QWidget *parent)
    : LayoutPanelBase(parent)
    , _tabWidget(new TabWidget(this))
{
	_mainLayout->addWidget(_tabWidget);
	_tabWidget->setDocumentMode(true);
	addTab(ContentPanelFactory::instance()->create("Empty", this));
}

QJsonObject qtengine::LayoutPanelTabber::serialize() const
{
	QJsonArray jsonStateTabs;
	for (int i = 0; i < _tabWidget->count(); i += 1)
		jsonStateTabs.append(dynamic_cast<ContentPanelBase *>(_tabWidget->widget(i))->serialize());

	QJsonObject jsonState;
	jsonState["CurrentTabberIndex"] = _tabWidget->currentIndex();
	jsonState["Tabs"] = jsonStateTabs;

	QJsonObject json;
	json["Type"] = "Tabber";
	json["State"] = jsonState;
	return json;
}

void qtengine::LayoutPanelTabber::deserialize(const QJsonObject &jsonState)
{
	for (int i = 0; i < _tabWidget->count(); i += 1)
		delete _tabWidget->closeTab(i);

	for (auto jsonStateTabRef : jsonState["Tabs"].toArray()) {
		QJsonObject jsonStateTab = jsonStateTabRef.toObject();
		ContentPanelBase *content = ContentPanelFactory::instance()->create(jsonStateTab["ContentPanelType"].toString(), this);

		if (!content)
			content = ContentPanelFactory::instance()->create("Empty", this);
		else
			content->deserialize(jsonStateTab);
		addTab(content);
	}
	_tabWidget->setCurrentIndex(jsonState["CurrentTabberIndex"].toInt());
}

int qtengine::LayoutPanelTabber::addTab(ContentPanelBase *contentPanel)
{
	return _tabWidget->QTabWidget::addTab(contentPanel, contentPanel->name());
}

int qtengine::LayoutPanelTabber::insertTab(const QPoint &pos, ContentPanelBase *contentPanel)
{
	int index = _tabWidget->tabAt(_tabWidget->mapFromGlobal(pos));

	return _tabWidget->insertTab(index, contentPanel, contentPanel->name());
}

qtengine::ContentPanelBase *qtengine::LayoutPanelTabber::closeTab(int index)
{
	auto panel = _tabWidget->closeTab(index);

	if (_tabWidget->count() == 0) {
		auto base = parentLayoutPanel();
		auto splitter = dynamic_cast<LayoutPanelSplitter*>(base);

		if (splitter)
			splitter->removeTabber(this);
		else if (base)
			base->setChild(new LayoutPanelTabber(base));
	}
	return panel;
}

QWidget *qtengine::LayoutPanelTabber::currentTab()
{
	return _tabWidget->currentWidget();
}

void qtengine::LayoutPanelTabber::setCurrentTab(int index)
{
	_tabWidget->setCurrentIndex(index);
}
