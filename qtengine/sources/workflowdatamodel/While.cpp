/*
** CODING JOURNEY
** workflow
** While
** LHUILE Léo
*/

#include "While.hpp"

#include "ClassTypeManager.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::While::While()
	: _flowControllerFill(false)
	, _conditionFill(false)
{
	refreshState();
}

QJsonObject qtengine::While::save() const
{
	QJsonObject json = QtNodes::NodeDataModel::save();

	json["isValid"] = true;
	json["nbrInput"] = static_cast<int>(nPorts(QtNodes::PortType::In));
	json["nbrOutput"] = static_cast<int>(nPorts(QtNodes::PortType::Out));
	json["code"] = code();
	json["objClassName"] = "";
	json["objName"] = "";
	return json;
}

void qtengine::While::restore(const QJsonObject &json)
{
	QtNodes::NodeDataModel::restore(json);
}

unsigned int qtengine::While::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = 2;
		break;
	case QtNodes::PortType::Out:
		ret = 2;
		break;
	}
	return ret;
}

std::shared_ptr<QtNodes::NodeData> qtengine::While::data(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	std::shared_ptr<QtNodes::NodeData> ret = std::shared_ptr<QtNodes::NodeData>(new QtNodes::NodeData());

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		if (portIndex == 0)
			ret = std::shared_ptr<QtNodes::NodeData>(new FlowController());
		else
			ret = std::shared_ptr<QtNodes::NodeData>(new Type(types::ClassTypeManager::instance()->type(QMetaType::Bool)));
		break;
	case QtNodes::PortType::Out:
		ret = std::shared_ptr<QtNodes::NodeData>(new FlowController());
		break;
	}
	return ret;
}

QString qtengine::While::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? "" : "condition";
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? "in" : "out";
		break;
	}
	return ret;
}

void qtengine::While::inputConnectionCreated(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = true;
	else
		_conditionFill = true;
	refreshState();
}

void qtengine::While::inputConnectionDeleted(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = false;
	else
		_conditionFill = false;
	refreshState();
}

void qtengine::While::refreshState()
{
	if (_flowControllerFill && _conditionFill) {
		setValidationState(QtNodes::NodeValidationState::Valid);
		setValidationMessage("");
	} else {
		setValidationState(QtNodes::NodeValidationState::Warning);
		setValidationMessage("Missing inputs");
	}
}

QString qtengine::While::code() const
{
	return
		"while (E_USEVAR(1)_E) {\n"
		"E_CODE(0)_E"
		"}\nE_CODE(1)_E";
}
