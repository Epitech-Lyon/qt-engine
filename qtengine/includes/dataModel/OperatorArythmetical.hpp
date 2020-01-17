/*
** CODING JOURNEY
** workflow
** OperatorArythmetical
** LHUILE Léo
*/

#pragma once

#include <QtWidgets/QComboBox>

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
			class OperatorArythmetical : public NodeDataModel {
				Q_OBJECT

			public:
				OperatorArythmetical();
				~OperatorArythmetical() = default;

			public:
				QJsonObject save() const override;
				void restore(QJsonObject const &) override;

				QString name() const override { return "OperatorArythmetical"; }

				QString caption() const override { return "Operator Operation"; }
				bool captionVisible() const override { return false; }

				unsigned int nPorts(PortType portType) const override;
				NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

				void setInData(std::shared_ptr<NodeData>, int) override;
				std::shared_ptr<NodeData> outData(PortIndex port) override;

				NodeValidationState validationState() const override { return _modelValidationState; }
				QString validationMessage() const override { return _modelValidationError; }

				QWidget *embeddedWidget() override { return _operators; }

			private:
				NodeValidationState _modelValidationState = NodeValidationState::Warning;
				QString _modelValidationError = "Missing inputs";
				QComboBox *_operators;
				std::pair<bool, std::shared_ptr<data::Number>> _inputs[2];
			};
		};
	};
};
