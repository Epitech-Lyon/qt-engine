/*
** CODING JOURNEY
** workflow
** Signal
** LHUILE Léo
*/

#include "Signal.hpp"
#include "types/includes/Signal.hpp"

#include "ObjectManager.hpp"

#include "SwitchButton.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"
#include "Node.hpp"
#include "Slot.hpp"

qtengine::Signal::Signal()
	: _connect(true)
	, _switchButton(new SwitchButton("Emit", "Connect"))
	, _flowControllerFill(false)
	, _flowControllerOutFill(false)
	, _errorSlot(false)
	, _signal(nullptr)
{
	connect(_switchButton, &SwitchButton::valueChanged, this, [this](bool value) {
		_connect = value;
		emit nodePortUpdated();
		refreshState();
	});
	_switchButton->setValue(_connect);
}

qtengine::Signal::~Signal()
{
	delete _signal;
}

void qtengine::Signal::setData(const QJsonObject &signalSave, const QUuid &objectId)
{
	_signal = new types::Signal;
	_signal->deserialize(signalSave);
	_signal->setContent(QJsonObject());
	if (_signal->name() == "objectNameChanged")
		_switchButton->setEnabled(false);
	_objectId = objectId;
	if (_signal->name() == "objectNameChanged")
		_switchButton->setEnabled(false);
	for (int i = 0; i < _signal->parameters().size(); i += 1)
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
	json["code"] = code();
	json["objClassName"] = libraryObjects::ObjectManager::instance()->objectClassName(_objectId);
	json["objName"] = libraryObjects::ObjectManager::instance()->objectName(_objectId);
	json["connect"] = _connect;
	json["classType"] = _signal->serialize();
	json["objectId"] = _objectId.toString();
	return json;
}

void qtengine::Signal::restore(const QJsonObject &json)
{
	_switchButton->setValue(json["connect"].toBool());
	setData(json["classType"].toObject(), QUuid(json["objectId"].toString()));
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
		ret = _connect ? 1 : _signal->parameters().count() + 1;
		break;
	case QtNodes::PortType::Out:
		ret = 1;
		break;
	}
	return ret;
}

std::shared_ptr<QtNodes::NodeData> qtengine::Signal::data(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	std::shared_ptr<QtNodes::NodeData> ret = std::shared_ptr<QtNodes::NodeData>(new QtNodes::NodeData());

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		if (portIndex == 0)
			ret = std::shared_ptr<QtNodes::NodeData>(new FlowController());
		else
			ret = std::shared_ptr<QtNodes::NodeData>(new Type(_signal->parameters()[portIndex - 1].first));
		break;
	case QtNodes::PortType::Out:
		ret = std::shared_ptr<QtNodes::NodeData>(new FlowController());
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
		ret = portIndex == 0 ? "" : data(portType, portIndex)->type().name + " " + _signal->parameters()[portIndex - 1].second;
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? "" : data(portType, portIndex)->type().name;
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

void qtengine::Signal::outputConnectionCreated(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerOutFill = true;
	if (_connect && portIndex == 0) {
		auto nodeDataModel = connection.getNode(QtNodes::PortType::In)->nodeDataModel();

		if (!dynamic_cast<Slot*>(nodeDataModel)) {
			setValidationState(QtNodes::NodeValidationState::Warning);
			setValidationMessage("Must be connected to a slot");
			_errorSlot = true;
		} else if (static_cast<int>(nodeDataModel->nPorts(QtNodes::PortType::In) - 1) != _signal->parameters().size()) {
			setValidationState(QtNodes::NodeValidationState::Warning);
			setValidationMessage("Signal and slot must have the same number of arguments");
			_errorSlot = true;
		} else
			for (unsigned int i = 1; i < nodeDataModel->nPorts(QtNodes::PortType::In); i += 1)
				if (!nodeDataModel->data(QtNodes::PortType::In, i)->sameType(std::shared_ptr<QtNodes::NodeData>(new Type(_signal->parameter(i - 1).first)))) {
					setValidationState(QtNodes::NodeValidationState::Warning);
					setValidationMessage("Signal and slot must have the same type of arguments");
					_errorSlot = true;
					break;
				}
	}
	refreshState();
}

void qtengine::Signal::outputConnectionDeleted(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerOutFill = false;
	_errorSlot = false;
	refreshState();
}

void qtengine::Signal::refreshState()
{
	if (_connect) {
		if (_flowControllerFill && _flowControllerOutFill && !_errorSlot) {
			setValidationState(QtNodes::NodeValidationState::Valid);
			setValidationMessage("");
		} else if (!_errorSlot) {
			setValidationState(QtNodes::NodeValidationState::Warning);
			setValidationMessage(_flowControllerOutFill ? "Missing inputs" : "Missing outputs");
		}
	} else {
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
}

QWidget *qtengine::Signal::embeddedWidget()
{
	return _switchButton;
}

QString qtengine::Signal::code() const
{
	QString ret;

	if (_connect) {
		ret += "connect(" + libraryObjects::ObjectManager::instance()->objectName(_objectId);
		ret += ", &" + libraryObjects::ObjectManager::instance()->objectClassName(_objectId) + "::" + _signal->name();
		ret += ", E_OBJNAME(0)_E, &E_OBJCLASSNAME(0)_E::E_SLOTNAME(0)_E);\nE_CODE(0)_E";
	} else {
		ret += "emit " + libraryObjects::ObjectManager::instance()->objectName(_objectId) + "->" + _signal->name() + "(";
		for (int i = 0; i < _inputsFill.size(); i += 1) {
			if (i > 0)
				ret += ", ";
			ret += "E_USEVAR(" + QString::number(i + 1) + ")_E";
		}
		ret += ");\nE_CODE(0)_E";
	}
	return ret;
}
