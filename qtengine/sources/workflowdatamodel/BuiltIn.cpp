/*
** CODING JOURNEY
** workflow
** BuiltIn
** LHUILE Léo
*/

#include "BuiltIn.hpp"
#include "Type.hpp"

#include "Connection.hpp"

#include "ClassTypeManager.hpp"

#include "QVariantConverter.hpp"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qtgroupboxpropertybrowser.h"
#include <QtWidgets/QGridLayout>

qtengine::BuiltIn::BuiltIn()
	: _type(QMetaType::UnknownType)
	, _propertyEditor(nullptr)
	, _propertyFactory(nullptr)
	, _propertyManager(nullptr)
{
}

void qtengine::BuiltIn::setData(QMetaType::Type type, const QVariant &value)
{
	_propertyEditor = new QtGroupBoxPropertyBrowser();
	_propertyFactory = new QtVariantEditorFactory(_propertyEditor);
	_propertyManager = new QtVariantPropertyManager(_propertyFactory);
	_propertyEditor->setFactoryForManager(_propertyManager, _propertyFactory);

	auto gridLayout = dynamic_cast<QGridLayout*>(_propertyEditor->layout());

	while (!gridLayout->isEmpty())
		delete gridLayout->takeAt(0);

	_type = type;
	_property = _propertyManager->addProperty(type, types::ClassTypeManager::instance()->type(_type));
	_propertyEditor->addProperty(_property);
	if (value.isValid())
		_property->setValue(value);
	gridLayout->setMargin(0);
	_propertyEditor->resize(_propertyEditor->sizeHint());
	emit embeddedWidgetChanged();
}

QJsonObject qtengine::BuiltIn::save() const
{
	QJsonObject json;

	json["name"] = "BUILTIN";
	json["isValid"] = true;
	json["nbrInput"] = static_cast<int>(nPorts(QtNodes::PortType::In));
	json["nbrOutput"] = static_cast<int>(nPorts(QtNodes::PortType::Out));
	json["code"] = code();
	json["objClassName"] = "";
	json["objName"] = "";
	json["type"] = static_cast<int>(_type);
	json["value"] = libraryObjects::QVariantConverter::serialize(_property->value());
	return json;
}

void qtengine::BuiltIn::restore(const QJsonObject &json)
{
	setData(static_cast<QMetaType::Type>(json["type"].toInt()), libraryObjects::QVariantConverter::deserialize(json["value"]));
}

QString qtengine::BuiltIn::name() const
{
	return _type != QMetaType::UnknownType ? types::ClassTypeManager::instance()->type(_type) : "BUILTIN";
}

QString qtengine::BuiltIn::caption() const
{
	return types::ClassTypeManager::instance()->type(_type);
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

std::shared_ptr<QtNodes::NodeData> qtengine::BuiltIn::data(QtNodes::PortType portType, QtNodes::PortIndex) const
{
	std::shared_ptr<QtNodes::NodeData> ret = std::shared_ptr<QtNodes::NodeData>(new QtNodes::NodeData());

	switch (portType) {
	case QtNodes::PortType::None:
		break;
	case QtNodes::PortType::In:
		break;
	case QtNodes::PortType::Out:
		ret = std::shared_ptr<QtNodes::NodeData>(new Type(types::ClassTypeManager::instance()->type(_type)));
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

QString qtengine::BuiltIn::code() const
{
	return libraryObjects::QVariantConverter::toString(_property->value());
}
