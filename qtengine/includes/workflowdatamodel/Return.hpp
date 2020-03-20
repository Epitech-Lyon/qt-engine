/*
** CODING JOURNEY
** workflow
** Return
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"

#include <QtCore/QList>

namespace qtengine {
	class Return : public QtNodes::NodeDataModel {
	public:
		Return(const QString &returnType);
		~Return() = default;

		bool canBeRemoved() const override { return false; }

		QJsonObject save() const override;
		void restore(const QJsonObject &json) override;

		QString name() const override { return "Return"; }

		QString caption() const override { return "Return"; }
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
		bool _flowControllerFill;
		bool _returnFill;
		QString _returnType;
	};
}
