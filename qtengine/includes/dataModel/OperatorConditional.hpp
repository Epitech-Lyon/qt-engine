/*
** CODING JOURNEY
** workflow
** OperatorConditional
** LHUILE Léo
*/

#pragma once

#include <QtCore/QObject>

#include "NodeDataModel.hpp"

#include <QtWidgets/QSpinBox>
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
			class OperatorConditional : public NodeDataModel {
				Q_OBJECT

			public:
				OperatorConditional();
				~OperatorConditional() = default;

			public:
				QString name() const override { return "OperatorConditional"; }

				QString caption() const override { return "Operator Conditional"; }
				bool captionVisible() const override { return false; }

				unsigned int nPorts(PortType portType) const override;
				bool portCaptionVisible(PortType, PortIndex) const override { return true; }
        		QString portCaption(PortType, PortIndex) const override;
				NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

				void setInData(std::shared_ptr<NodeData> data, int) override;
				std::shared_ptr<NodeData> outData(PortIndex port) override;

				NodeValidationState validationState() const override;
				QString validationMessage() const override;

				QWidget *embeddedWidget() override { return nullptr; }

			private:
				NodeValidationState _modelValidationState = NodeValidationState::Warning;
				QString _modelValidationError = "Missing inputs";
				QSpinBox *_inputsEdit;
                std::vector<bool> _inputs;
			};
		};
	};
};
