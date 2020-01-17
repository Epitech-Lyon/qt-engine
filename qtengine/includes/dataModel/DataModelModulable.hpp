/*
** CODING JOURNEY
** workflow
** Modulable
** LHUILE Léo
*/

#pragma once

#include <vector>

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
			class Modulable : public NodeDataModel {
			public:
				Modulable();
				~Modulable() = default;

				QString name() const override { return _name; }

				QString caption() const override { return _caption; }
				bool captionVisible() const override { return true; }

				unsigned int nPorts(PortType portType) const override;
				NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

				QString portCaption(PortType, PortIndex) const override;
				bool portCaptionVisible(PortType, PortIndex) const { return true; }

				void setInData(std::shared_ptr<NodeData> data, int) override;
				std::shared_ptr<NodeData> outData(PortIndex port) override;

				QWidget *embeddedWidget() override { return nullptr; }

				NodeValidationState validationState() const override;
				QString validationMessage() const override;

				void setName(const QString &name) { _name = name; }
				void setCaption(const QString &caption) { _caption = caption; }
				void setInputs(const std::vector<std::pair<QString, QString>> &inputs) { _inputs = inputs; }
				void setOutputs(const std::vector<std::pair<QString, QString>> &outputs) { _outputs = outputs; }

			private:
				NodeValidationState _modelValidationState = NodeValidationState::Warning;
				QString _modelValidationError = "Missing inputs";

				QString _name;
				QString _caption;
				std::vector<std::pair<QString, QString>> _inputs;
				std::vector<std::pair<QString, QString>> _outputs;
			};
		};
	};
};
