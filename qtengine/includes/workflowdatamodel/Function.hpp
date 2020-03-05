/*
** CODING JOURNEY
** workflow
** Function
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"
#include <QtCore/QMetaMethod>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;
using QtNodes::Connection;

namespace qtengine {
	class Function : public NodeDataModel {
	public:
		Function(const QMetaMethod &metaMethod);
		~Function() = default;

		QString name() const override { return _metaMethod.name(); }

		QString caption() const override { return _metaMethod.name(); }
		bool captionVisible() const override { return true; }

		unsigned int nPorts(PortType portType) const override;
		NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

		QString portCaption(PortType portType, PortIndex portIndex) const override;
		bool portCaptionVisible(PortType, PortIndex) const override { return true; }

		ConnectionPolicy portOutConnectionPolicy(PortIndex portIndex) const override;

	public slots:
		void inputConnectionCreated(Connection const &) override;
		void inputConnectionDeleted(Connection const &) override;

	private:
		void refreshState();
		bool _flowControllerFill;
		QVector<bool> _inputsFill;
		QMetaMethod _metaMethod;
	};
}
