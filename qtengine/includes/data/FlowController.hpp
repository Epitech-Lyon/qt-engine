/*
** CODING JOURNEY
** workflow
** FlowController
** LHUILE Léo
*/

#pragma once

#include "NodeData.hpp"

namespace codingJourney {
	namespace workflow {
		namespace data {
			class FlowController : public QtNodes::NodeData {
			public:
				FlowController() = default;

				QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType {"FlowController", "FlowController"}; }
			};
		};
	};
};
