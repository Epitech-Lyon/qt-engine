/*
** CODING JOURNEY
** workflow
** Method
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"
#include <QtCore/QMetaMethod>

namespace qtengine {
	class Method : public QtNodes::NodeDataModel {
	public:
		Method(const QMetaMethod &metaMethod);
		~Method() = default;

		QString name() const override { return _metaMethod.name(); }

		QString caption() const override { return _metaMethod.name(); }
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
		QMetaMethod _metaMethod;
	};
}
