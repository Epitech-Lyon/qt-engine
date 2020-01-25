/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelEmpty
*/

#pragma once

#include "ContentPanelBase.hpp"

namespace qtengine {
	class ContentPanelEmpty : public ContentPanelBase {
	public:
		ContentPanelEmpty(QWidget * = nullptr);
		~ContentPanelEmpty() = default;
	};
}
