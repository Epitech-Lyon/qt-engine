/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelTabber
*/

#include "LayoutPanelTabber.hpp"

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
}

QJsonObject qtengine::LayoutPanelTabber::serialize() const
{
	QJsonArray jsonStateTabs;
	for (int i = 0; i < _tabWidget->count(); i += 1)
		jsonStateTabs.append(dynamic_cast<ContentPanelBase *>(_tabWidget->widget(i))->serialize());

	QJsonObject jsonState;
	jsonState["CurrentTabIndex"] = _tabWidget->currentIndex();
	jsonState["Tabs"] = jsonStateTabs;

	QJsonObject json;
	json["Type"] = "Tabber";
	json["State"] = jsonState;
	return json;
}

void qtengine::LayoutPanelTabber::deserialize(const QJsonObject &jsonState)
{
	if (jsonState.contains("Tabs") && jsonState["Tabs"].isArray()) {
		QJsonArray jsonStateTabs = jsonState["Tabs"].toArray();

		for (int i = 0; i < jsonStateTabs.size(); i += 1) {
			QJsonObject json = jsonStateTabs[i].toObject();

			if (json.contains("ContentPanelType") && json["ContentPanelType"].isString())  {
				ContentPanelBase *content = ContentPanelFactory::create(json["ContentPanelType"].toString());

				content->deserialize(json);
				addView(content);
			}
		}
	}
	if (jsonState.contains("CurrentTabberIndex") && jsonState["CurrentTabberIndex"].isDouble())
		_tabWidget->setCurrentIndex(jsonState["CurrentTabberIndex"].toInt());
}

QWidget *qtengine::LayoutPanelTabber::currentView()
{
	return _tabWidget->currentWidget();
}

void qtengine::LayoutPanelTabber::setCurrentView(int index)
{
	_tabWidget->setCurrentIndex(index);
}

int qtengine::LayoutPanelTabber::insertView(const QPoint &pos, ContentPanelBase *contentPanel)
{
	int index = _tabWidget->tabAt(_tabWidget->mapFromGlobal(pos));

	return _tabWidget->insertTab(index, contentPanel, contentPanel->name());
}

int qtengine::LayoutPanelTabber::addView(ContentPanelBase *contentPanel)
{
	return _tabWidget->QTabWidget::addTab(contentPanel, contentPanel->name());
}

void qtengine::LayoutPanelTabber::removeView(int index)
{
	_tabWidget->closeTab(index);
}
