/*
** CODING JOURNEY
** workflow
** Constructor
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"

namespace types {
	class Constructor;
}

namespace qtengine {
	class Constructor : public QtNodes::NodeDataModel {
	public:
		Constructor();
		~Constructor();

		void setData(const QJsonObject &constructorSave);

		QJsonObject save() const override;
		void restore(const QJsonObject &json) override;

		QString name() const override;

		QString caption() const override;
		bool captionVisible() const override { return true; }

		unsigned int nPorts(QtNodes::PortType portType) const override;
		QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

		QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override { return true; }

		QtNodes::NodeDataModel::ConnectionPolicy portOutConnectionPolicy(QtNodes::PortIndex portIndex) const override;

	public slots:
		void inputConnectionCreated(QtNodes::Connection const &) override;
		void inputConnectionDeleted(QtNodes::Connection const &) override;

	private:
		void refreshState();
		bool _flowControllerFill;
		QVector<bool> _inputsFill;
		types::Constructor *_constructor;
	};
}
