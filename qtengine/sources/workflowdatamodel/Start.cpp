/*
** CODING JOURNEY
** workflow
** Start
** LHUILE Léo
*/

#include "Start.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

qtengine::Start::Start(const QList<QPair<QString, QString>> &parameters)
	: _parameters(parameters)
{
}

QJsonObject qtengine::Start::save() const
{
	QJsonObject json = QtNodes::NodeDataModel::save();

	json["isValid"] = true;
	return json;
}

void qtengine::Start::restore(const QJsonObject &json)
{
	QtNodes::NodeDataModel::restore(json);
}

unsigned int qtengine::Start::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = 0;
		break;
	case QtNodes::PortType::Out:
		ret = 1 + _parameters.size();
		break;
	}
	return ret;
}

QtNodes::NodeDataType qtengine::Start::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QtNodes::NodeDataType ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? FlowController().type() : Type(_parameters[portIndex - 1].first).type();
		break;
	}
	return ret;
}

QString qtengine::Start::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? "" : _parameters[portIndex - 1].first + " " + _parameters[portIndex - 1].second;
		break;
	}
	return ret;
}

QtNodes::NodeDataModel::ConnectionPolicy qtengine::Start::portOutConnectionPolicy(QtNodes::PortIndex portIndex) const
{
	return portIndex > 0 ? ConnectionPolicy::Many : ConnectionPolicy::One;
}
