/*
** CODING JOURNEY
** workflow
** Property
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"
#include <QtCore/QMetaProperty>

namespace types {
	class Property;
}

namespace qtengine {
	class SwitchButton;

	class Property : public QtNodes::NodeDataModel {
	public:
		Property();
		~Property();

		void setData(const QJsonObject &propertySave, const QUuid &objectId);

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

		QWidget *embeddedWidget() override;

	public slots:
		void inputConnectionCreated(QtNodes::Connection const &) override;
		void inputConnectionDeleted(QtNodes::Connection const &) override;

	private:
		void refreshState();
		QString code() const;

		bool _needBtn;
		bool _get;
		SwitchButton *_switchButton;
		bool _flowControllerFill;
		bool _valueFill;
		types::Property *_property;
		QUuid _objectId;
	};
}
