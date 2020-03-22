/*
** CODING JOURNEY
** workflow
** Property
** LHUILE Léo
*/

#include "Property.hpp"
#include "types/includes/Property.hpp"

#include "ObjectManager.hpp"

#include "SwitchButton.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::Property::Property()
	: _needBtn(true)
	, _get(true)
	, _switchButton(new SwitchButton("Set", "Get"))
	, _flowControllerFill(false)
	, _valueFill(false)
	, _property(nullptr)
{
	connect(_switchButton, &SwitchButton::valueChanged, this, [this](bool value) {
		_get = value;
		emit nodePortUpdated();
		refreshState();
	});
	_switchButton->setValue(_get);
}

qtengine::Property::~Property()
{
	delete _property;
}

void qtengine::Property::setData(const QJsonObject &propertySave, const QUuid &objectId)
{
	_property = new types::Property;
	_property->deserialize(propertySave);
	_property->setContent(QJsonObject());
	_needBtn = !_property->setterName().isEmpty() && !_property->getterName().isEmpty();
	_get = _needBtn ? _get : !_property->getterName().isEmpty() ? true : false;
	_objectId = objectId;
	emit embeddedWidgetChanged();
}

QJsonObject qtengine::Property::save() const
{
	QJsonObject json;

	json["name"] = QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::PROPERTY);
	json["isValid"] = validationState() == QtNodes::NodeValidationState::Valid;
	json["nbrInput"] = static_cast<int>(nPorts(QtNodes::PortType::In));
	json["nbrOutput"] = static_cast<int>(nPorts(QtNodes::PortType::Out));
	json["code"] = code();
	json["objClassName"] = libraryObjects::ObjectManager::instance()->objectClassName(_objectId);
	json["objName"] = libraryObjects::ObjectManager::instance()->objectName(_objectId);
	json["get"] = _get;
	json["classType"] = _property->serialize();
	json["objectId"] = _objectId.toString();
	return json;
}

void qtengine::Property::restore(const QJsonObject &json)
{
	_switchButton->setValue(json["get"].toBool());
	setData(json["classType"].toObject(), QUuid(json["objectId"].toString()));
}

QString qtengine::Property::name() const
{
	return _property ? _property->name() : QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::PROPERTY);
}

QString qtengine::Property::caption() const
{
	return _property->signature();
}

unsigned int qtengine::Property::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = _get ? _property->isUserType() ? 0 : 1 : 2;
		break;
	case QtNodes::PortType::Out:
		ret = _get ? _property->isUserType() ? 1 : 2 : 1;
		break;
	}
	return ret;
}

QtNodes::NodeDataType qtengine::Property::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QtNodes::NodeDataType ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? FlowController().type() : Type(_property->type()).type();
		break;
	case QtNodes::PortType::Out:
		ret = !_get || !_property->isUserType() ? FlowController().type() : Type(_property->type()).type();
		break;
	}
	return ret;
}

QString qtengine::Property::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = portIndex == 0 ? "" : "set";
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? "" : "get";
		break;
	}
	return ret;
}

QtNodes::NodeDataModel::ConnectionPolicy qtengine::Property::portOutConnectionPolicy(QtNodes::PortIndex portIndex) const
{
	return portIndex > 0 ? ConnectionPolicy::Many : ConnectionPolicy::One;
}

void qtengine::Property::inputConnectionCreated(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = true;
	else
		_valueFill = true;
	refreshState();
}

void qtengine::Property::inputConnectionDeleted(QtNodes::Connection const &connection)
{
	int portIndex = connection.getPortIndex(QtNodes::PortType::In);

	if (portIndex == 0)
		_flowControllerFill = false;
	else
		_valueFill = false;
	refreshState();
}

void qtengine::Property::refreshState()
{
	if (_get || (_flowControllerFill && _valueFill)) {
		setValidationState(QtNodes::NodeValidationState::Valid);
		setValidationMessage("");
	} else {
		setValidationState(QtNodes::NodeValidationState::Warning);
		setValidationMessage("Missing inputs");
	}
}

QWidget *qtengine::Property::embeddedWidget()
{
	return _needBtn ? _switchButton : nullptr;
}

QString qtengine::Property::code() const
{
	QString ret;

	if (_get) {
		if (_property->isUserType()) {
			ret += "E_VAR(" + _property->name() + ")_E";
			ret += libraryObjects::ObjectManager::instance()->objectName(_objectId, true) + "->";
			ret += _property->getterName() + "()";
		} else {
			ret += _property->type() + " E_VAR()_E = ";
			ret += libraryObjects::ObjectManager::instance()->objectName(_objectId, true) + "->";
			ret += _property->getterName() + "(" + _property->name() + ");\nE_CODE(0)_E";
		}
	} else {
		if (_property->isUserType()) {
			ret += libraryObjects::ObjectManager::instance()->objectName(_objectId, true) + "->";
			ret += _property->setterName() + "(E_USEVAR(1)_E);\nE_CODE(0)_E";
		} else {
			ret += libraryObjects::ObjectManager::instance()->objectName(_objectId, true) + "->";
			ret += _property->setterName() + "(" + _property->name() + "QVariant::fromValue(E_USEVAR(1)_E));\nE_CODE(0)_E";
		}
	}
	return ret;
}
