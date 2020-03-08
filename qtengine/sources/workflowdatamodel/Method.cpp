/*
** CODING JOURNEY
** workflow
** Method
** LHUILE Léo
*/

#include "Method.hpp"
#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::Method::Method(const QMetaMethod &metaMethod)
	: _flowControllerFill(false)
	, _metaMethod(metaMethod)
{
	for (unsigned int i = 0; i < nPorts(QtNodes::PortType::In); i += 1)
		_inputsFill << false;
	refreshState();
}

unsigned int qtengine::Method::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = _metaMethod.parameterCount() + 1;
		break;
	case QtNodes::PortType::Out:
		ret = _metaMethod.returnType() == QMetaType::Void ? 1 : 2;
		break;
	}
	return ret;
}

QtNodes::NodeDataType qtengine::Method::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QtNodes::NodeDataType ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? FlowController().type() : Type(_metaMethod.parameterType(portIndex - 1)).type();
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? FlowController().type() : Type(_metaMethod.returnType()).type();
		break;
	}
	return ret;
}

QString qtengine::Method::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? "" : dataType(portType, portIndex).name + " " + _metaMethod.parameterNames()[portIndex - 1];
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? "" : dataType(portType, portIndex).name;
		break;
	}
	return ret;
}

QtNodes::NodeDataModel::ConnectionPolicy qtengine::Method::portOutConnectionPolicy(QtNodes::PortIndex portIndex) const
{
	return portIndex > 0 ? ConnectionPolicy::Many : ConnectionPolicy::One;
}

void qtengine::Method::inputConnectionCreated(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = true;
	else
		_inputsFill[portIndex - 1] = true;
	refreshState();
}

void qtengine::Method::inputConnectionDeleted(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = false;
	else
		_inputsFill[portIndex - 1] = false;
	refreshState();
}

void qtengine::Method::refreshState()
{
	auto allFilled = true;

	for (auto inputFill : _inputsFill)
		allFilled = allFilled && inputFill;
	if (_flowControllerFill && allFilled) {
		setValidationState(QtNodes::NodeValidationState::Valid);
		setValidationMessage("");
	} else {
		setValidationState(QtNodes::NodeValidationState::Warning);
		setValidationMessage("Missing inputs");
	}
}
