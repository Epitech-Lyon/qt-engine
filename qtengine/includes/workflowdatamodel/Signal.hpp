/*
** CODING JOURNEY
** workflow
** Signal
** LHUILE Léo
*/

#pragma once

#include "NodeDataModel.hpp"

namespace types {
	class Signal;
}

namespace qtengine {
	class SwitchButton;

	class Signal : public QtNodes::NodeDataModel {
	public:
		Signal();
		~Signal();

		void setData(const QJsonObject &signalSave, const QUuid &objectId);
		bool connectToSlot() const { return _connect; }

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

		QWidget *embeddedWidget() override;

	public slots:
		void inputConnectionCreated(QtNodes::Connection const &) override;
		void inputConnectionDeleted(QtNodes::Connection const &) override;
		void outputConnectionCreated(QtNodes::Connection const &) override;
		void outputConnectionDeleted(QtNodes::Connection const &) override;

	private:
		void refreshState();
		QString code() const;

		bool _connect;
		SwitchButton *_switchButton;
		bool _flowControllerFill;
		QVector<bool> _inputsFill;
		bool _flowControllerOutFill;
		bool _errorSlot;
		types::Signal *_signal;
		QUuid _objectId;
	};
}
