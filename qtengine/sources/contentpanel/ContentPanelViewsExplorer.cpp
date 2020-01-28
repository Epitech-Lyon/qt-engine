/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewsExplorer
*/

#include "ContentPanelViewsExplorer.hpp"

qtengine::ContentPanelViewsExplorer::ContentPanelViewsExplorer(QWidget *parent)
	: ContentPanelBase("Views Explorer", parent)
{
}

void qtengine::ContentPanelViewsExplorer::init()
{
	_treeWidget = new QTreeWidget(this);
	_mainLayout->addWidget(_treeWidget);
}

QToolBar *qtengine::ContentPanelViewsExplorer::initToolBar()
{
	return nullptr;
}
