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
	mainMenu->addAction(QIcon(":icon_close"), "Close tab", this, &TabWidget::closeCurrentTab);
	mainMenu->addSeparator();
	mainMenu->addAction(QIcon(":icon_splitter_vertical"), "Split vertical", this, &TabWidget::splitVertical);
	mainMenu->addAction(QIcon(":icon_splitter_horizontal"), "Split horizontal", this, &TabWidget::splitHorizontal);
	mainMenu->addSeparator();

	auto menuSetTabs = mainMenu->addMenu("Set tab");
	auto menuAddTabs = mainMenu->addMenu("Add tab");
	for (auto name: ContentPanelFactory::getNames()) {
		menuSetTabs->addAction(name, [&]() { setTab(name); });
		menuAddTabs->addAction(name, [&]() { addTab(name); });
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

	QTabWidget::addTab(contentPanel, name);
	return contentPanel;
}

qtengine::ContentPanelBase *qtengine::TabWidget::setTab(const QString &name)
{
	int currentIndex = this->currentIndex();
	auto contentPanel = ContentPanelFactory::create(name);

	delete currentWidget();
	insertTab(currentIndex, contentPanel, name);
	setCurrentIndex(currentIndex);
	return contentPanel;
}

void qtengine::TabWidget::closeTab(int index)
{
	auto tabber = dynamic_cast<LayoutPanelTabber*>(this->parentWidget());
	auto splitter = dynamic_cast<LayoutPanelSplitter *>(tabber->parentLayoutPanel());

	if (splitter || (!splitter && count() > 1))
		removeTab(index);
	if (count() > 0 || !splitter)
		return;
	setParent(nullptr);
	auto parent = splitter->parentLayoutPanel();
	auto child = (splitter->firstLayoutPanel() == tabber) ? splitter->secondLayoutPanel() : splitter->firstLayoutPanel();

	if (dynamic_cast<LayoutPanelSplitter *>(parent)) {
		if (dynamic_cast<LayoutPanelSplitter *>(parent)->firstLayoutPanel() == splitter)
			dynamic_cast<LayoutPanelSplitter *>(parent)->setFirstLayoutPanel(child);
		else
			dynamic_cast<LayoutPanelSplitter *>(parent)->setSecondLayoutPanel(child);
	} else
		parent->setChildPanel(child);
	delete splitter;
}

void qtengine::TabWidget::closeCurrentTab()
{
	int currentTabIndex = currentIndex();

	auto *tabber = dynamic_cast<LayoutPanelTabber*>(this->parentWidget());
	auto *splitter = dynamic_cast<LayoutPanelSplitter *>(tabber->parentLayoutPanel());

	if (splitter || (!splitter && count() > 1))
		delete widget(currentTabIndex);
	if (count() > 0 || !splitter)
		return;
	auto parent = splitter->parentLayoutPanel();
	auto child = splitter->firstLayoutPanel() == tabber ? splitter->secondLayoutPanel() : splitter->firstLayoutPanel();

	if (dynamic_cast<LayoutPanelSplitter *>(parent)) {
		if (dynamic_cast<LayoutPanelSplitter *>(parent)->firstLayoutPanel() == splitter)
			dynamic_cast<LayoutPanelSplitter *>(parent)->setFirstLayoutPanel(child);
		else
			dynamic_cast<LayoutPanelSplitter *>(parent)->setSecondLayoutPanel(child);
	} else
		parent->setChildPanel(child);
	delete splitter;
}

void qtengine::TabWidget::splitVertical()
{
	LayoutPanelTabber* tabber = dynamic_cast<LayoutPanelTabber*>(this->parentWidget());
	if (!tabber) { return; }

	LayoutPanelBase *parent = tabber->parentLayoutPanel();
	LayoutPanelSplitter *newSplitter = new LayoutPanelSplitter(parent);

	newSplitter->setOrientation(Qt::Vertical);
	if (dynamic_cast<LayoutPanelSplitter*>(parent)) {
		int index = dynamic_cast<LayoutPanelSplitter*> (parent)->indexOf(tabber);

		dynamic_cast<LayoutPanelSplitter*>(parent)->setChildPanel(index, newSplitter);
		newSplitter->setChildrenPanel(tabber, new LayoutPanelTabber(newSplitter));
	} else {
		dynamic_cast<LayoutPanelBase*>(parent)->setChildPanel(newSplitter);
		newSplitter->setChildrenPanel(tabber, new LayoutPanelTabber(newSplitter));
	}
}

void qtengine::TabWidget::splitHorizontal()
{
	LayoutPanelTabber* tabber = dynamic_cast<LayoutPanelTabber*>(this->parentWidget());
	if (!tabber) { return; }

	LayoutPanelBase* parent = tabber->parentLayoutPanel();
	LayoutPanelSplitter* newSplitter = new LayoutPanelSplitter(parent);

	newSplitter->setOrientation(Qt::Horizontal);
	if (dynamic_cast<LayoutPanelSplitter*>(parent)) {
		int index = dynamic_cast<LayoutPanelSplitter*>(parent)->indexOf(tabber);
		dynamic_cast<LayoutPanelSplitter*>(parent)->setChildPanel(index, newSplitter);

		newSplitter->setChildrenPanel(tabber, new LayoutPanelTabber(newSplitter));
	} else {
		dynamic_cast<LayoutPanelBase*>(parent)->setChildPanel(newSplitter);
		newSplitter->setChildrenPanel(tabber, new LayoutPanelTabber(newSplitter));
	}
}
