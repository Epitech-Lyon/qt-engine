/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TabWidget
*/

#include "TabWidget.hpp"

#include <QtWidgets/QToolButton>
#include "ContentPanelFactory.hpp"
#include "LayoutPanelTabber.hpp"
#include "LayoutPanelSplitter.hpp"
#include "ContentPanelBase.hpp"
#include "TabBar.hpp"

qtengine::TabWidget::TabWidget(QWidget *parent)
	: QTabWidget(parent)
{
	setDocumentMode(true);
	setTabBar(new TabBar);
	initButton();
}

int qtengine::TabWidget::tabAt(const QPoint &pos)
{
	return tabBar()->tabAt(pos);
}

qtengine::ContentPanelBase *qtengine::TabWidget::getPage(int index) const
{
	return dynamic_cast<ContentPanelBase *>(widget(index));
}

void qtengine::TabWidget::initButton()
{
	auto toolButton = new QToolButton(this);

	auto mainMenu = new QMenu(toolButton);
	mainMenu->addAction(QIcon(":icon_close"), "Close tab", [this]() { dynamic_cast<LayoutPanelTabber*>(parentWidget())->closeTab(currentIndex()); });
	mainMenu->addSeparator();
	mainMenu->addAction(QIcon(":icon_splitter_vertical"), "Split vertical", [this]() { split(Qt::Vertical); });
	mainMenu->addAction(QIcon(":icon_splitter_horizontal"), "Split horizontal", [this]() { split(Qt::Horizontal); });
	mainMenu->addSeparator();

	auto menuSetTabs = mainMenu->addMenu("Set tab");
	auto menuAddTabs = mainMenu->addMenu("Add tab");
	for (auto name: ContentPanelFactory::getNames()) {
		menuSetTabs->addAction(name, [this, name]() { setTab(name); });
		menuAddTabs->addAction(name, [this, name]() { addTab(name); });
	}

	toolButton->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
	toolButton->setPopupMode(QToolButton::InstantPopup);
	toolButton->setAutoRaise(true);
	toolButton->setMenu(mainMenu);
	toolButton->setArrowType(Qt::ArrowType::DownArrow);
	toolButton->adjustSize();
	setCornerWidget(toolButton);
}

qtengine::ContentPanelBase *qtengine::TabWidget::addTab(const QString &name)
{
	auto contentPanel = ContentPanelFactory::create(name);

	setCurrentIndex(QTabWidget::addTab(contentPanel, name));
	return contentPanel;
}

qtengine::ContentPanelBase *qtengine::TabWidget::setTab(const QString &name)
{
	int index = currentIndex();
	auto contentPanel = ContentPanelFactory::create(name);

	currentWidget()->deleteLater();
	removeTab(index);
	setCurrentIndex(insertTab(index, contentPanel, name));
	return contentPanel;
}

qtengine::ContentPanelBase *qtengine::TabWidget::closeTab(int index)
{
	auto widget = this->widget(index);

	removeTab(index);
	return dynamic_cast<ContentPanelBase *>(widget);
}

void qtengine::TabWidget::split(Qt::Orientation orientation)
{
	LayoutPanelTabber* tabber = dynamic_cast<LayoutPanelTabber*>(parentWidget());
	if (!tabber) { return; }
	auto base = tabber->parentLayoutPanel();
	auto splitter = dynamic_cast<LayoutPanelSplitter*>(base);

	if (splitter)
		splitter->split(orientation, tabber);
	else if (base) {
		auto newSplitter = new LayoutPanelSplitter(tabber, orientation, base);

		base->setChild(newSplitter);
	}
}
