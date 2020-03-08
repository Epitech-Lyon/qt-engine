/*
** CODING JOURNEY
** workflow
** Property
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"
#include <QtCore/QMetaProperty>

namespace qtengine {
	class Property : public QtNodes::NodeDataModel {
	public:
		Property(const QMetaProperty &metaProperty);
		~Property() = default;

		QString name() const override { return _metaProperty.name(); }

		QString caption() const override { return QString(_metaProperty.typeName()) + " " + _metaProperty.name(); }
		bool captionVisible() const override { return true; }

		unsigned int nPorts(QtNodes::PortType portType) const override;
		QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

		QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override { return true; }

		QtNodes::NodeDataModel::ConnectionPolicy portOutConnectionPolicy(QtNodes::PortIndex portIndex) const override;

	private:
		QMetaProperty _metaProperty;
	};
}
