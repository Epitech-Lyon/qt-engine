/*
** CODING JOURNEY
** workflow
** Boolean
** LHUILE Léo
*/

#pragma once

#include "NodeData.hpp"

namespace codingJourney {
	namespace workflow {
		namespace data {
			class Boolean : public QtNodes::NodeData {
			public:
				Boolean() : _value(false) {}
				Boolean(const bool value) : _value(value) {}
				Boolean(const Boolean &boolean) : _value(boolean.value()) {}

				QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType {"boolean", "Boolean"}; }

				bool value() const { return _value; }

			private:
				bool _value;
			};
		};
	};
};
