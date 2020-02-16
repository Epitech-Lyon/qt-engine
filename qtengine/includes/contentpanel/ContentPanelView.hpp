/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelView
*/

#pragma once

#include "ContentPanelBase.hpp"

namespace libraryObjects {
	class AObject;	
}

namespace qtengine {
	class ContentPanelView : public ContentPanelBase {
	public:
		ContentPanelView(QWidget *parent = nullptr);
		~ContentPanelView();

		void init() override;

	private slots:
		void onViewObjectChanged(libraryObjects::AObject *object);

	private:
		void clear();
	};
}
