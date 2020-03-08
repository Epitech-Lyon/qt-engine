/*
** CODING JOURNEY
** workflow
** Property
** LHUILE Léo
*/

#include "Property.hpp"
#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

qtengine::Property::Property(const QMetaProperty &metaProperty)
	: _metaProperty(metaProperty)
{
}

unsigned int qtengine::Property::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		ret = _metaProperty.isWritable() + 1;
		break;
	case QtNodes::PortType::Out:
		ret = _metaProperty.isReadable() + 1;
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
		ret = portIndex == 0 ? FlowController().type() : Type(_metaProperty.type()).type();
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? FlowController().type() : Type(_metaProperty.type()).type();
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
