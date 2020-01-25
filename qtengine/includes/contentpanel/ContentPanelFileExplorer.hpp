/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelFileExplorer
*/

#pragma once

#include "ContentPanelBase.hpp"

namespace qtengine {
	class ContentPanelFileExplorer : public ContentPanelBase {
		public:
			ContentPanelFileExplorer(QWidget * = nullptr);
			~ContentPanelFileExplorer() = default;

			void init() override;

		private:
	};
}
