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
	class Property : public QtNodes::NodeDataModel {
	public:
		Property();
		~Property();

		void setData(const QJsonObject &propertySave, const QString &objectId);

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

	private:
		types::Property *_property;
		QString _objectId;
	};
}
