/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Function
*/

#pragma once

#include "NodeDataModel.hpp"

#include "ClassTypeManager.hpp"

namespace qtengine {
	class Function : public QtNodes::NodeDataModel {
	public:
		Function();
		~Function() = default;

		void setData(const types::ClassTypeManager::Function &functon);

		QString name() const override { return _functionIsNull ? "FUNCTION" : _function.name; }

		QJsonObject save() const override;
		void restore(const QJsonObject &json) override;

		QString caption() const override { return _function.name; }
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
		QString code() const { return _function.code; }

		bool _flowControllerFill;
		QVector<bool> _inputsFill;
		types::ClassTypeManager::Function _function;
		bool _functionIsNull;
	};
}
