/*
** CODING JOURNEY
** workflow
** Function
** LHUILE Léo
*/

#include "Function.hpp"
#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::Function::Function(const QMetaMethod &metaMethod)
	: _flowControllerFill(false)
	, _metaMethod(metaMethod)
{
	for (unsigned int i = 0; i < nPorts(PortType::In); i += 1)
		_inputsFill << false;
	refreshState();
}

unsigned int qtengine::Function::nPorts(PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
		ret = _metaMethod.parameterCount() + 1;
		break;
	case PortType::Out:
		ret = _metaMethod.returnType() == QMetaType::Void ? 1 : 2;
		break;
	}
	return ret;
}

NodeDataType qtengine::Function::dataType(PortType portType, PortIndex portIndex) const
{
	NodeDataType ret;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
		ret = portIndex == 0 ? FlowController().type() : Type(_metaMethod.parameterType(portIndex - 1)).type();
		break;
	case PortType::Out:
		ret = portIndex == 0 ? FlowController().type() : Type(_metaMethod.returnType()).type();
		break;
	}
	return ret;
}

QString qtengine::Function::portCaption(PortType portType, PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
		ret = portIndex == 0 ? "" : dataType(portType, portIndex).name + " " + _metaMethod.parameterNames()[portIndex - 1];
		break;
	case PortType::Out:
		ret = portIndex == 0 ? "" : dataType(portType, portIndex).name;
		break;
	}
	return ret;
}

QtNodes::NodeDataModel::ConnectionPolicy qtengine::Function::portOutConnectionPolicy(PortIndex portIndex) const
{
	return portIndex > 0 ? ConnectionPolicy::Many : ConnectionPolicy::One;
}

void qtengine::Function::inputConnectionCreated(Connection const &connection)
{
	int portIndex = connection.getPortIndex(PortType::In);

	if (portIndex == 0)
		_flowControllerFill = true;
	else
		_inputsFill[portIndex - 1] = true;
	refreshState();
}

void qtengine::Function::inputConnectionDeleted(Connection const &connection)
{
	int portIndex = connection.getPortIndex(PortType::In);

	if (portIndex == 0)
		_flowControllerFill = false;
	else
		_inputsFill[portIndex - 1] = false;
	refreshState();
}

void qtengine::Function::refreshState()
{
	auto allFilled = true;

	for (auto inputFill : _inputsFill)
		allFilled = allFilled && inputFill;
	if (_flowControllerFill && allFilled) {
		setValidationState(NodeValidationState::Valid);
		setValidationMessage("");
	} else {
		setValidationState(NodeValidationState::Warning);
		setValidationMessage("Missing inputs");
	}
}
