/*
** CODING JOURNEY
** workflow
** NumberDisplay
** LHUILE Léo
*/

#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include "NodeDataModel.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

namespace codingJourney {
	namespace workflow {
		namespace dataModel {
			class NumberDisplay : public NodeDataModel {
			public:
				NumberDisplay();
				~NumberDisplay() = default;

			public:
				QString name() const override { return "NumberDisplay"; }

				QString caption() const override { return "Number display"; }
				bool captionVisible() const override { return false; }

				unsigned int nPorts(PortType portType) const override;
				NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

				void setInData(std::shared_ptr<NodeData> data, int) override;
				std::shared_ptr<NodeData> outData(PortIndex port) override;

				QWidget *embeddedWidget() override { return _label; }

				NodeValidationState validationState() const override;
				QString validationMessage() const override;

			private:
				NodeValidationState _modelValidationState = NodeValidationState::Warning;
				QString _modelValidationError = "Missing inputs";
				QLabel *_label;
			};
		};
	};
};
