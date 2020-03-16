/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelLibraryObject
*/

#pragma once

#include "ContentPanelBase.hpp"

namespace qtengine {
	class TreeWidgetLibraryObject;

	class ContentPanelLibraryObject : public ContentPanelBase {
	public:
		ContentPanelLibraryObject(QWidget *parent = nullptr);
		~ContentPanelLibraryObject() = default;

		void init() override;

	private:
		TreeWidgetLibraryObject *_tree;
	};
}
