/*
** CODING JOURNEY
** workflow
** Method
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"
#include <QtCore/QUuid>

namespace types {
	class Method;
}

namespace qtengine {
	class Method : public QtNodes::NodeDataModel {
	public:
		Method();
		~Method();

		void setData(const QJsonObject &methodSave, const QUuid &objectId);

		QJsonObject save() const override;
		void restore(const QJsonObject &json) override;

		QString name() const override;

		QString caption() const override;
		bool captionVisible() const override { return true; }

		unsigned int nPorts(QtNodes::PortType portType) const override;
		std::shared_ptr<QtNodes::NodeData> data(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

		QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override { return true; }

		QtNodes::NodeDataModel::ConnectionPolicy portOutConnectionPolicy(QtNodes::PortIndex portIndex) const override;

	public slots:
		void inputConnectionCreated(QtNodes::Connection const &) override;
		void inputConnectionDeleted(QtNodes::Connection const &) override;

	private:
		void refreshState();
		QString code() const;

		bool _flowControllerFill;
		QVector<bool> _inputsFill;
		types::Method *_method;
		QUuid _objectId;
	};
}
