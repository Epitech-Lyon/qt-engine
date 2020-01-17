/*
** CODING JOURNEY
** workflow
** BooleanSource
** LHUILE Léo
*/

#pragma once

#include <QtWidgets/QPushButton>

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
			class Boolean;
		};
		namespace dataModel {
			class BooleanSource : public NodeDataModel {
				Q_OBJECT

			public:
				BooleanSource();
				~BooleanSource() = default;

			public:
				QJsonObject save() const override;
				void restore(QJsonObject const &) override;

				QString name() const override { return "BooleanSource"; }

				QString caption() const override { return "Boolean Source"; }
				bool captionVisible() const override { return false; }

				unsigned int nPorts(PortType portType) const override;
				NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

				void setInData(std::shared_ptr<NodeData>, int) override {}
				std::shared_ptr<NodeData> outData(PortIndex port) override;

				QWidget *embeddedWidget() override { return _button; }

			private:
				QPushButton *_button;
				std::shared_ptr<data::Boolean> _boolean;
			};
		};
	};
};
