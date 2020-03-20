/*
** CODING JOURNEY
** workflow
** Method
** LHUILE Léo
*/

#include "Method.hpp"
#include "types/includes/Method.hpp"

#include "ClassTypeManager.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::Method::Method()
	: _flowControllerFill(false)
	, _method(nullptr)
{
}

qtengine::Method::~Method()
{
	delete _method;
}

void qtengine::Method::setData(const QJsonObject &methodSave, const QString &objectId)
{
	_method = new types::Method();
	_method->deserialize(methodSave);
	_method->setContent(QJsonObject());
	_objectId = objectId;
	for (unsigned int i = 0; i < nPorts(QtNodes::PortType::In) - 1; i += 1)
		_inputsFill << false;
	refreshState();
}

QJsonObject qtengine::Method::save() const
{
	QJsonObject json;

	json["name"] = QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::METHOD);
	json["isValid"] = validationState() == QtNodes::NodeValidationState::Valid;
	json["classType"] = _method->serialize();
	json["objectId"] = _objectId;
	return json;
}

void qtengine::Method::restore(const QJsonObject &json)
{
	setData(json["classType"].toObject(), json["objectId"].toString());
}

QString qtengine::Method::name() const
{
	return _method ? _method->name() : QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::METHOD);
}

QString qtengine::Method::caption() const
{
	return _method->signature();
}

unsigned int qtengine::Method::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = _method->parameters().count() + 1;
		break;
	case QtNodes::PortType::Out:
		ret = _method->returnType() == types::ClassTypeManager::instance()->type(QMetaType::Void) ? 1 : 2;
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
		ret = portIndex == 0 ? FlowController().type() : Type(_method->parameters()[portIndex - 1].first).type();
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? FlowController().type() : Type(_method->returnType()).type();
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
		ret = portIndex == 0 ? "" : dataType(portType, portIndex).name + " " + _method->parameters()[portIndex - 1].second;
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
