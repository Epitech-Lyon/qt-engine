/*
** CODING JOURNEY
** workflow
** OperatorAND
** LHUILE Léo
*/

#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include "NodeDataModel.hpp"
#include <vector>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

namespace codingJourney {
	namespace workflow {
		namespace data {
			class Boolean;
		};
		namespace dataModel {
			class OperatorAND : public NodeDataModel {
			public:
				OperatorAND();
				~OperatorAND() = default;

			public:
				QString name() const override { return "OperatorAND"; }

				QString caption() const override { return "Operator AND"; }
				bool captionVisible() const override { return false; }

				unsigned int nPorts(PortType portType) const override;
				NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

				bool portCaptionVisible(PortType, PortIndex) const override { return true; }

				void setInData(std::shared_ptr<NodeData> data, int) override;
				std::shared_ptr<NodeData> outData(PortIndex port) override;

				NodeValidationState validationState() const override { return _modelValidationState; }
				QString validationMessage() const override { return _modelValidationError; }

				QWidget *embeddedWidget() override { return _label; }

			private:
				NodeValidationState _modelValidationState = NodeValidationState::Warning;
				QString _modelValidationError = "Missing inputs";
				QLabel *_label;
				std::pair<bool, std::shared_ptr<data::Boolean>> _inputs[2];
			};
		};
	};
};
