/*
** CODING JOURNEY
** workflow
** Modulable
** LHUILE Léo
*/

#pragma once

#include <vector>

#include "FlowController.hpp"
#include "Boolean.hpp"
#include "Number.hpp"

namespace codingJourney {
	namespace workflow {
		namespace data {
			class Modulable {
			public:
				static NodeDataType getDataTypeOf(const QString &dataName)
				{
					static Modulable modulable;

					for (auto &node : modulable._nodes)
						if (node.type().name == dataName)
							return node.type();
					return NodeDataType {};
				}

			private:
				Modulable()
				{
//					_nodes.push_back(FlowController());
//					_nodes.push_back(Boolean());
//					_nodes.push_back(Number());
				}

				std::vector<NodeData> _nodes;
			};
		};
	};
};
