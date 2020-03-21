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

		QJsonObject save() const override;
		void restore(const QJsonObject &json) override;

		QString name() const override { return "START"; }

		QString caption() const override { return "START"; }
		bool captionVisible() const override { return true; }

		unsigned int nPorts(QtNodes::PortType portType) const override;
		QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

		QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override { return true; }

		QtNodes::NodeDataModel::ConnectionPolicy portOutConnectionPolicy(QtNodes::PortIndex portIndex) const override;

	private:
		QString code() const;

		QList<QPair<QString, QString>> _parameters;
	};
}
