/*
** CODING JOURNEY
** workflow
** NumberSource
** LHUILE Léo
*/

#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QSpinBox>

#include "NodeDataModel.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

namespace codingJourney {
	namespace workflow {
		namespace data {
			class Number;
		};
		namespace dataModel {
			class NumberSource : public NodeDataModel {
				Q_OBJECT

			public:
				NumberSource();
				~NumberSource() = default;

			public:
				QJsonObject save() const override;
				void restore(QJsonObject const &) override;

				QString name() const override { return "NumberSource"; }

				QString caption() const override { return "Number Source"; }
				bool captionVisible() const override { return false; }

				unsigned int nPorts(PortType portType) const override;
				NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

				void setInData(std::shared_ptr<NodeData>, int) override {}
				std::shared_ptr<NodeData> outData(PortIndex port) override;

				QWidget *embeddedWidget() override { return _numberEdit; }

			private:
				QSpinBox *_numberEdit;

				std::shared_ptr<data::Number> _number;
			};
		};
	};
};
