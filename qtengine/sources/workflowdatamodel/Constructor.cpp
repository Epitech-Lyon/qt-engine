/*
** CODING JOURNEY
** workflow
** Constructor
** LHUILE Léo
*/

#include "Constructor.hpp"
#include "types/includes/Constructor.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::Constructor::Constructor()
	: _flowControllerFill(false)
	, _constructor(nullptr)
{
}

qtengine::Constructor::~Constructor()
{
	delete _constructor;
}

void qtengine::Constructor::setData(const QJsonObject &constructorSave)
{
	_constructor = new types::Constructor;
	_constructor->deserialize(constructorSave);
	_constructor->setContent(QJsonObject());
	for (unsigned int i = 0; i < nPorts(QtNodes::PortType::In) - 1; i += 1)
		_inputsFill << false;
	refreshState();
}

QJsonObject qtengine::Constructor::save() const
{
	QJsonObject json;

	json["name"] = QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::CONSTRUCTOR);
	json["isValid"] = validationState() == QtNodes::NodeValidationState::Valid;
	json["nbrInput"] = static_cast<int>(nPorts(QtNodes::PortType::In));
	json["nbrOutput"] = static_cast<int>(nPorts(QtNodes::PortType::Out));
	json["code"] = code();
	json["objClassName"] = "";
	json["objName"] = "";
	json["classType"] = _constructor->serialize();
	return json;
}

void qtengine::Constructor::restore(const QJsonObject &json)
{
	setData(json["classType"].toObject());
}

QString qtengine::Constructor::name() const
{
	return _constructor ? _constructor->signature() : QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::CONSTRUCTOR);
}

QString qtengine::Constructor::caption() const
{
	return _constructor->signature();
}

unsigned int qtengine::Constructor::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = _constructor->parameters().count() + 1;
		break;
	case QtNodes::PortType::Out:
		ret = 2;
		break;
	}
	return ret;
}

std::shared_ptr<QtNodes::NodeData> qtengine::Constructor::data(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	std::shared_ptr<QtNodes::NodeData> ret = std::shared_ptr<QtNodes::NodeData>(new QtNodes::NodeData());

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		if (portIndex == 0)
			ret = std::shared_ptr<QtNodes::NodeData>(new FlowController());
		else
			ret = std::shared_ptr<QtNodes::NodeData>(new Type(_constructor->parameters()[portIndex - 1].first));
		break;
	case QtNodes::PortType::Out:
		if (portIndex == 0)
			ret = std::shared_ptr<QtNodes::NodeData>(new FlowController());
		else
			ret = std::shared_ptr<QtNodes::NodeData>(new Type(_constructor->className() + "*"));
		break;
	}
	return ret;
}

QString qtengine::Constructor::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? "" : data(portType, portIndex)->type().name + " " + _constructor->parameters()[portIndex - 1].second;
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? "" : data(portType, portIndex)->type().name;
		break;
	}
	return ret;
}

QtNodes::NodeDataModel::ConnectionPolicy qtengine::Constructor::portOutConnectionPolicy(QtNodes::PortIndex portIndex) const
{
	return portIndex > 0 ? ConnectionPolicy::Many : ConnectionPolicy::One;
}

void qtengine::Constructor::inputConnectionCreated(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = true;
	else
		_inputsFill[portIndex - 1] = true;
	refreshState();
}

void qtengine::Constructor::inputConnectionDeleted(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = false;
	else
		_inputsFill[portIndex - 1] = false;
	refreshState();
}

void qtengine::Constructor::refreshState()
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

QString qtengine::Constructor::code() const
{
	QString ret = _constructor->className() + " *E_VAR()_E = new " + _constructor->className() + "(";

	for (int i = 0; i < _inputsFill.size(); i += 1) {
		if (i > 0)
			ret += ", ";
		ret += "E_USEVAR(" + QString::number(i + 1) + ")_E";
	}
	ret += ");\nE_CODE(0)_E";
	return ret;
}
