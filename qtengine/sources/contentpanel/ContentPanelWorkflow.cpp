/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#include "ContentPanelWorkflow.hpp"

#include "FlowView.hpp"
#include "FlowScene.hpp"

qtengine::ContentPanelWorkflow::ContentPanelWorkflow(QWidget *parent)
	: ContentPanelBase("Workflow", parent)
{
}

void qtengine::ContentPanelWorkflow::init()
{
	_scene = new QtNodes::FlowScene();
	_view = new QtNodes::FlowView(_scene, this);

	_mainLayout->addWidget(_view);
}
