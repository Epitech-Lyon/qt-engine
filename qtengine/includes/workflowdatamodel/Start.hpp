/*
** CODING JOURNEY
** workflow
** Start
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"

#include <QtCore/QList>

namespace qtengine {
	class Start : public QtNodes::NodeDataModel {
	public:
		Start(const QList<QPair<QString, QString>> &parameters);
		~Start() = default;

		bool canBeRemoved() const override { return false; }

		QString name() const override { return "Start"; }

		QString caption() const override { return "Start"; }
		bool captionVisible() const override { return true; }

		unsigned int nPorts(QtNodes::PortType portType) const override;
		QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

		QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override { return true; }

		QtNodes::NodeDataModel::ConnectionPolicy portOutConnectionPolicy(QtNodes::PortIndex portIndex) const override;

	private:
		QList<QPair<QString, QString>> _parameters;
	};
}
