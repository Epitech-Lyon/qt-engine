/*
** CODING JOURNEY
** workflow
** Property
** LHUILE Léo
*/

#include "Property.hpp"
#include "types/includes/Property.hpp"

#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::Property::Property()
	: _property(nullptr)
{
}

qtengine::Property::~Property()
{
	delete _property;
}

void qtengine::Property::setData(const QJsonObject &propertySave, const QString &objectId)
{
	_property = new types::Property;
	_property->deserialize(propertySave);
	_objectId = objectId;
}

QJsonObject qtengine::Property::save() const
{
	QJsonObject json;

	json["name"] = QMetaEnum::fromType<types::ClassType::Type>().key(types::ClassType::PROPERTY);
	json["classType"] = _property->serialize();
	json["objectId"] = _objectId;
	return json;
}

void qtengine::Property::restore(const QJsonObject &json)
{
	setData(json["classType"].toObject(), json["objectId"].toString());
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
		ret = !_property->setterName().isEmpty() + 1;
		break;
	case QtNodes::PortType::Out:
		ret = !_property->getterName().isEmpty() + 1;
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
		ret = portIndex == 0 ? FlowController().type() : Type(_property->type()).type();
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
