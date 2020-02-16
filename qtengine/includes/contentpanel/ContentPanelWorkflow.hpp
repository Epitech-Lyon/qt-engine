/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#pragma once

#include "ContentPanelBase.hpp"

namespace QtNodes {
	class FlowView;
	class FlowScene;
};

namespace qtengine {
	class ContentPanelWorkflow : public ContentPanelBase {
	public:
		ContentPanelWorkflow(QWidget *parent = nullptr);
		~ContentPanelWorkflow() = default;

		void init() override;

	private:
		QtNodes::FlowView *_view;
		QtNodes::FlowScene *_scene;
	};
}
