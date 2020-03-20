/*
** CODING JOURNEY
** workflow
** Signal
** LHUILE Léo
*/

#include "Signal.hpp"
#include "types/includes/Signal.hpp"

#include "SwitchButton.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::Signal::Signal()
	: _connect(true)
	, _switchButton(new SwitchButton("Emit", "Connect"))
	, _flowControllerFill(false)
	, _signal(nullptr)
{
	connect(_switchButton, &SwitchButton::valueChanged, this, [this](bool value) {
		_connect = value;
		refreshState();
	});
	_switchButton->setValue(_connect);
}

qtengine::Signal::~Signal()
{
	delete _signal;
}

void qtengine::Signal::setData(const QJsonObject &signalSave, const QString &objectId)
{
	_signal = new types::Signal;
	_signal->deserialize(signalSave);
	_signal->setContent(QJsonObject());
	_objectId = objectId;
	for (unsigned int i = 0; i < nPorts(QtNodes::PortType::In) - 1; i += 1)
		_inputsFill << false;
	refreshState();
}

QJsonObject qtengine::Signal::save() const
{
	QJsonObject json;

	json["name"] = QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::SIGNAL);
	json["isValid"] = validationState() == QtNodes::NodeValidationState::Valid;
	json["nbrInput"] = static_cast<int>(nPorts(QtNodes::PortType::In));
	json["nbrOutput"] = static_cast<int>(nPorts(QtNodes::PortType::Out));
	json["connect"] = _connect;
	json["classType"] = _signal->serialize();
	json["objectId"] = _objectId;
	return json;
}

void qtengine::Signal::restore(const QJsonObject &json)
{
	_switchButton->setValue(json["connect"].toBool());
	setData(json["classType"].toObject(), json["objectId"].toString());
}

QString qtengine::Signal::name() const
{
	return _signal ? _signal->name() : QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::SIGNAL);
}

QString qtengine::Signal::caption() const
{
	return _signal->signature();
}

unsigned int qtengine::Signal::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = _signal->parameters().count() + 1;
		break;
	case QtNodes::PortType::Out:
		ret = 1;
		break;
	}
	return ret;
}

QtNodes::NodeDataType qtengine::Signal::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QtNodes::NodeDataType ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? FlowController().type() : Type(_signal->parameters()[portIndex - 1].first).type();
		break;
	case QtNodes::PortType::Out:
		ret = FlowController().type();
		break;
	}
	return ret;
}

QString qtengine::Signal::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? "" : dataType(portType, portIndex).name + " " + _signal->parameters()[portIndex - 1].second;
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? "" : dataType(portType, portIndex).name;
		break;
	}
	return ret;
}

QtNodes::NodeDataModel::ConnectionPolicy qtengine::Signal::portOutConnectionPolicy(QtNodes::PortIndex portIndex) const
{
	return portIndex > 0 ? ConnectionPolicy::Many : ConnectionPolicy::One;
}

void qtengine::Signal::inputConnectionCreated(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = true;
	else
		_inputsFill[portIndex - 1] = true;
	refreshState();
}

void qtengine::Signal::inputConnectionDeleted(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = false;
	else
		_inputsFill[portIndex - 1] = false;
	refreshState();
}

void qtengine::Signal::refreshState()
{
	auto allFilled = true;

	if (!_connect)
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

QWidget *qtengine::Signal::embeddedWidget()
{
	return _switchButton;
}
