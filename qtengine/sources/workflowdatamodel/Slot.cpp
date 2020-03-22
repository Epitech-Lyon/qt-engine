/*
** CODING JOURNEY
** workflow
** Slot
** LHUILE Léo
*/

#include "Slot.hpp"
#include "types/includes/Slot.hpp"

#include "ObjectManager.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"
#include "Node.hpp"
#include "Signal.hpp"

qtengine::Slot::Slot()
	: _flowControllerFill(false)
	, _connect(false)
	, _slot(nullptr)
{
}

qtengine::Slot::~Slot()
{
	delete _slot;
}

void qtengine::Slot::setData(const QJsonObject &slotSave, const QUuid &objectId)
{
	_slot = new types::Slot;
	_slot->deserialize(slotSave);
	_slot->setContent(QJsonObject());
	_objectId = objectId;
	for (unsigned int i = 0; i < nPorts(QtNodes::PortType::In) - 1; i += 1)
		_inputsFill << false;
	refreshState();
}

QJsonObject qtengine::Slot::save() const
{
	QJsonObject json;

	json["name"] = QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::SLOT);
	json["isValid"] = validationState() == QtNodes::NodeValidationState::Valid;
	json["nbrInput"] = static_cast<int>(nPorts(QtNodes::PortType::In));
	json["nbrOutput"] = static_cast<int>(nPorts(QtNodes::PortType::Out));
	json["code"] = code();
	json["objClassName"] = libraryObjects::ObjectManager::instance()->objectClassName(_objectId);
	json["objName"] = libraryObjects::ObjectManager::instance()->objectName(_objectId);
	json["classType"] = _slot->serialize();
	json["objectId"] = _objectId.toString();
	return json;
}

void qtengine::Slot::restore(const QJsonObject &json)
{
	setData(json["classType"].toObject(), QUuid(json["objectId"].toString()));
}

QString qtengine::Slot::name() const
{
	return _slot ? _slot->name() : QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::SLOT);
}

QString qtengine::Slot::caption() const
{
	return _slot->signature();
}

unsigned int qtengine::Slot::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = _connect ? 1 : _slot->parameters().count() + 1;
		break;
	case QtNodes::PortType::Out:
		ret = 1;
		break;
	}
	return ret;
}

QtNodes::NodeDataType qtengine::Slot::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QtNodes::NodeDataType ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? FlowController().type() : Type(_slot->parameters()[portIndex - 1].first).type();
		break;
	case QtNodes::PortType::Out:
		ret = FlowController().type();
		break;
	}
	return ret;
}

QString qtengine::Slot::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? "" : dataType(portType, portIndex).name + " " + _slot->parameters()[portIndex - 1].second;
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? "" : dataType(portType, portIndex).name;
		break;
	}
	return ret;
}

QtNodes::NodeDataModel::ConnectionPolicy qtengine::Slot::portOutConnectionPolicy(QtNodes::PortIndex portIndex) const
{
	return portIndex > 0 ? ConnectionPolicy::Many : ConnectionPolicy::One;
}

void qtengine::Slot::inputConnectionCreated(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0) {
		_flowControllerFill = true;
		auto signalDataModel = dynamic_cast<Signal*>(connection.getNode(QtNodes::PortType::Out)->nodeDataModel());
		_connect = signalDataModel && signalDataModel->connectToSlot();
	} else
		_inputsFill[portIndex - 1] = true;
	refreshState();
}

void qtengine::Slot::inputConnectionDeleted(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0) {
		_flowControllerFill = false;
		_connect = false;
	} else
		_inputsFill[portIndex - 1] = false;
	refreshState();
}

void qtengine::Slot::refreshState()
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

QString qtengine::Slot::code() const
{
	QString ret;
	
	if (_connect) {
		ret += "E_CODE(0)_E";
	} else {
		ret += libraryObjects::ObjectManager::instance()->objectName(_objectId) + "->" + _slot->name() + "(";
		for (int i = 0; i < _inputsFill.size(); i += 1) {
			if (i > 0)
				ret += ", ";
			ret += "E_USEVAR(" + QString::number(i + 1) + ")_E";
		}
		ret += ");\nE_CODE(0)_E";
	}
	return ret;
}
