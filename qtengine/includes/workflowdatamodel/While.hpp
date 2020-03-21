/*
** CODING JOURNEY
** workflow
** While
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"

#include <QtCore/QList>

namespace qtengine {
	class While : public QtNodes::NodeDataModel {
	public:
		While();
		~While() = default;

		QJsonObject save() const override;
		void restore(const QJsonObject &json) override;

		QString name() const override { return "WHILE"; }

		QString caption() const override { return "WHILE"; }
		bool captionVisible() const override { return true; }

		unsigned int nPorts(QtNodes::PortType portType) const override;
		QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

		QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override { return true; }

	public slots:
		void inputConnectionCreated(QtNodes::Connection const &) override;
		void inputConnectionDeleted(QtNodes::Connection const &) override;

	private:
		void refreshState();
		QString code() const;

		bool _flowControllerFill;
		bool _conditionFill;
	};
}
