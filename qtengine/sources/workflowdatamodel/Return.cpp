/*
** CODING JOURNEY
** workflow
** Return
** LHUILE Léo
*/

#include "Return.hpp"

#include "ClassTypeManager.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::Return::Return(const QString &returnType)
	: _flowControllerFill(false)
	, _returnFill(returnType == types::ClassTypeManager::instance()->type(QMetaType::Void))
	, _returnType(returnType)
{
	refreshState();
}

QJsonObject qtengine::Return::save() const
{
	QJsonObject json = QtNodes::NodeDataModel::save();

	json["isValid"] = validationState() == QtNodes::NodeValidationState::Valid;
	return json;
}

void qtengine::Return::restore(const QJsonObject &json)
{
	QtNodes::NodeDataModel::restore(json);
}

unsigned int qtengine::Return::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = _returnType == types::ClassTypeManager::instance()->type(QMetaType::Void) ? 1 : 2;
		break;
	case QtNodes::PortType::Out:
		break;
	}
	return ret;
}

QtNodes::NodeDataType qtengine::Return::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QtNodes::NodeDataType ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? FlowController().type() : Type(_returnType).type();
		break;
	case QtNodes::PortType::Out:
		break;
	}
	return ret;
}

QString qtengine::Return::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? "": dataType(portType, portIndex).name;
		break;
	case QtNodes::PortType::Out:
		break;
	}
	return ret;
}

void qtengine::Return::inputConnectionCreated(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = true;
	else
		_returnFill = true;
	refreshState();
}

void qtengine::Return::inputConnectionDeleted(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = false;
	else
		_returnFill = false;
	refreshState();
}

void qtengine::Return::refreshState()
{
	if (_flowControllerFill && _returnFill) {
		setValidationState(QtNodes::NodeValidationState::Valid);
		setValidationMessage("");
	} else {
		setValidationState(QtNodes::NodeValidationState::Warning);
		setValidationMessage("Missing inputs");
	}
}
