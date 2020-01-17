/*
** CODING JOURNEY
** workflow
** Number
** LHUILE Léo
*/

#pragma once

#include "NodeData.hpp"

using QtNodes::NodeDataType;
using QtNodes::NodeData;

namespace codingJourney {
	namespace workflow {
		namespace data {
			class Number : public QtNodes::NodeData {
			public:
				Number() : _value(0) {}
				Number(const int value) : _value(value) {}
				Number(const Number &number) : _value(number.value()) {}

				QtNodes::NodeDataType type() const override { return QtNodes::NodeDataType {"number", "Number"}; }

				int value() const { return _value; }

			private:
				int _value;
			};
		};
	};
};
