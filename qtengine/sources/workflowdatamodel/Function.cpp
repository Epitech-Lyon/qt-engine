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

#include <QtCore/QJsonArray>

qtengine::Function::Function()
	: _flowControllerFill(false)
	, _functionIsNull(true)
{
}

void qtengine::Function::setData(const types::ClassTypeManager::Function &function)
{
	_function = function;
	_functionIsNull = false;
	for (unsigned int i = 0; i < nPorts(QtNodes::PortType::In) - 1; i += 1)
		_inputsFill << false;
	refreshState();
}

QJsonObject qtengine::Function::save() const
{
	QJsonArray jsonFunctionInputs;
	for (auto input : _function.inputs)
		jsonFunctionInputs.append(input);

	QJsonObject jsonFunction;
	jsonFunction["inputs"] = jsonFunctionInputs;
	jsonFunction["output"] = _function.output;
	jsonFunction["name"] = _function.name;
	jsonFunction["code"] = _function.code;

	QJsonObject json;
	json["name"] = "FUNCTION";
	json["isValid"] = validationState() == QtNodes::NodeValidationState::Valid;
	json["nbrInput"] = static_cast<int>(nPorts(QtNodes::PortType::In));
	json["nbrOutput"] = static_cast<int>(nPorts(QtNodes::PortType::Out));
	json["code"] = code();
	json["objClassName"] = "";
	json["objName"] = "";
	json["function"] = jsonFunction;
	return json;
}

void qtengine::Function::restore(const QJsonObject &json)
{
	types::ClassTypeManager::Function function;
	QJsonObject jsonFunction = json["function"].toObject();
	QJsonArray jsonFunctionIntputs = jsonFunction["inputs"].toArray();

	for (auto inputRef : jsonFunctionIntputs)
		function.inputs.append(inputRef.toString());
	function.output = jsonFunction["output"].toString();
	function.name = jsonFunction["name"].toString();
	function.code = jsonFunction["code"].toString();
	setData(function);
}

unsigned int qtengine::Function::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = _function.inputs.size() + 1;
		break;
	case QtNodes::PortType::Out:
		ret = _function.output == types::ClassTypeManager::instance()->type(QMetaType::Void) ? 1 : 2;
		break;
	}
	return ret;
}

std::shared_ptr<QtNodes::NodeData> qtengine::Function::data(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	std::shared_ptr<QtNodes::NodeData> ret = std::shared_ptr<QtNodes::NodeData>(new QtNodes::NodeData());

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		if (portIndex == 0)
			ret = std::shared_ptr<QtNodes::NodeData>(new FlowController());
		else
			ret = std::shared_ptr<QtNodes::NodeData>(new Type(_function.inputs[portIndex - 1]));
		break;
	case QtNodes::PortType::Out:
		if (portIndex == 0)
			ret = std::shared_ptr<QtNodes::NodeData>(new FlowController());
		else
			ret = std::shared_ptr<QtNodes::NodeData>(new Type(_function.output));
		break;
	}
	return ret;
}

QString qtengine::Function::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? "" : data(portType, portIndex)->type().name;
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? "" : data(portType, portIndex)->type().name;
		break;
	}
	return ret;
}

QtNodes::NodeDataModel::ConnectionPolicy qtengine::Function::portOutConnectionPolicy(QtNodes::PortIndex portIndex) const
{
	return portIndex > 0 ? ConnectionPolicy::Many : ConnectionPolicy::One;
}

void qtengine::Function::inputConnectionCreated(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = true;
	else
		_inputsFill[portIndex - 1] = true;
	refreshState();
}

void qtengine::Function::inputConnectionDeleted(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

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
		setValidationState(QtNodes::NodeValidationState::Valid);
		setValidationMessage("");
	} else {
		setValidationState(QtNodes::NodeValidationState::Warning);
		setValidationMessage("Missing inputs");
	}
}
