/*
** CODING JOURNEY
** workflow
** BuiltIn
** LHUILE Léo
*/

#include "BuiltIn.hpp"
#include "FlowController.hpp"
#include "Type.hpp"

#include "Connection.hpp"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qtgroupboxpropertybrowser.h"
#include <QtWidgets/QGridLayout>

qtengine::BuiltIn::BuiltIn(QVariant::Type type)
	: _type(type)
	, _propertyEditor(new QtGroupBoxPropertyBrowser())
	, _propertyFactory(new QtVariantEditorFactory(_propertyEditor))
	, _propertyManager(new QtVariantPropertyManager(_propertyFactory))
{
	auto gridLayout = dynamic_cast<QGridLayout*>(_propertyEditor->layout());

	while (!gridLayout->isEmpty())
		delete gridLayout->takeAt(0);

	_propertyEditor->setFactoryForManager(_propertyManager, _propertyFactory);
	_propertyEditor->addProperty(_propertyManager->addProperty(type, QVariant::typeToName(_type)));
	gridLayout->setMargin(0);
	_propertyEditor->resize(_propertyEditor->sizeHint());
}

unsigned int qtengine::BuiltIn::nPorts(QtNodes::PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		break;
	case QtNodes::PortType::Out:
		ret = 1;
		break;
	}
	return ret;
}

QtNodes::NodeDataType qtengine::BuiltIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QtNodes::NodeDataType ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		break;
	case QtNodes::PortType::Out:
		ret = portIndex == 0 ? FlowController().type() : Type(_type).type();
		break;
	}
	return ret;
}

QString qtengine::BuiltIn::portCaption(QtNodes::PortType portType, QtNodes::PortIndex) const
{
	QString ret;

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		break;
	case QtNodes::PortType::Out:
		break;
	}
	return ret;
}

QtNodes::NodeDataModel::ConnectionPolicy qtengine::BuiltIn::portOutConnectionPolicy(QtNodes::PortIndex portIndex) const
{
	return portIndex > 0 ? ConnectionPolicy::Many : ConnectionPolicy::One;
}

QWidget *qtengine::BuiltIn::embeddedWidget()
{
	return _propertyEditor;
}
