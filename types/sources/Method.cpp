/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Method
*/

#include "moc_Method.cpp"
#include "Method.hpp"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qtgroupboxpropertybrowser.h"

#include "Utils.hpp"
#include <QtCore/QDebug>

types::Method::Method()
	: ClassType(QMetaMethod::Access::Public, Type::METHOD)
	, _isStatic(false)
	, _returnType(QMetaType::Type::Void)
	, _isConst(false)
{
}

types::Method::Method(const QMetaMethod &metaMethod)
	: ClassType(metaMethod.access(), Type::METHOD)
	, _isStatic(false)
	, _returnType(static_cast<QMetaType::Type>(metaMethod.returnType()))
	, _name(metaMethod.name())
	, _isConst(false)
{
	for (auto parameterName : metaMethod.parameterNames())
		addParameter(static_cast<QMetaType::Type>(metaMethod.parameterType(_parameters.size())), parameterName);
}

QJsonObject types::Method::serialize() const
{
	QJsonArray jsonParameters;
	for (const auto &parameter : _parameters) {
		QJsonObject jsonParameter;
		jsonParameter["type"] = static_cast<int>(parameter.first);
		jsonParameter["name"] = parameter.first;
		jsonParameters.append(jsonParameter);
	}

	QJsonObject json;
	json["access"] = static_cast<int>(_access);
	json["isStatic"] = _isStatic;
	json["returnType"] = static_cast<int>(_returnType);
	json["name"] = _name;
	json["parameters"] = jsonParameters;
	json["isConst"] = _isConst;
	return json;
}

void types::Method::deserialize(const QJsonObject &json)
{
	_access = static_cast<QMetaMethod::Access>(json["access"].toInt());
	_isStatic = json["isStatic"].toBool();
	_returnType = static_cast<QMetaType::Type>(json["returnType"].toInt());
	_name = json["name"].toString();
	for (auto jsonParameterRef : json["parameters"].toArray()) {
		auto jsonParameter = jsonParameterRef.toObject();

		addParameter(static_cast<QMetaType::Type>(jsonParameter["type"].toInt()), jsonParameter["name"].toString());
	}
	_isConst = json["isConst"].toBool();
}

QWidget *types::Method::initEditor()
{
	auto propertyEditor = new QtGroupBoxPropertyBrowser();
	auto propertyFactory = new QtVariantEditorFactory(propertyEditor);
	auto propertyManager = new QtVariantPropertyManager(propertyFactory);

	propertyEditor->setFactoryForManager(propertyManager, propertyFactory);

	auto propertySlot = new QMap<QtProperty *, std::function<void (const QVariant &)>>;
	{
		auto property = propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), "Access");
		QStringList accessString;

		for (auto access : {QMetaMethod::Private, QMetaMethod::Protected, QMetaMethod::Public})
			accessString << types::accessToString(access);
		property->setAttribute("enumNames", accessString);
		property->setValue(accessString.indexOf(types::accessToString(_access)));
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this](const QVariant &value) {
			setAccess(static_cast<QMetaMethod::Access>(value.toInt()));
		};
	}
	{
		auto property = propertyManager->addProperty(QVariant::Bool, "Is static");

		property->setValue(_isStatic);
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this](const QVariant &value) {
			setStatic(value.toBool());
		};
	}
	{
		QMap<QString, QMetaType::Type> types;
		for (int i = QMetaType::UnknownType + 1; i < QMetaType::User + 1; i += 1)
			if (QMetaType::isRegistered(i) && i != QMetaType::Void)
				types[QMetaType::typeName(i)] = static_cast<QMetaType::Type>(i);
		auto property = propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), "Return type");

		property->setAttribute("enumNames", QStringList(types.keys()));
		property->setValue(types.keys().indexOf(QMetaType::typeName(_returnType)));
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this, types](const QVariant &value) {
			setReturnType(types.values()[value.toInt()]);
		};
	}
	{
		auto property = propertyManager->addProperty(QVariant::String, "Name");

		property->setValue(_name);
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this](const QVariant &value) {
			setName(value.toString());
		};
	}
	{
		auto propertyGroup = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), "Parameters");
		auto addParameter = [this, propertyManager, propertyEditor, propertySlot, propertyGroup](QMetaType::Type type, const QString &name) {
			this->addParameter(type, name);
			auto propertySubGroup = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId());
			{
				QMap<QString, QMetaType::Type> types;
				for (int i = QMetaType::UnknownType + 1; i < QMetaType::User + 1; i += 1)
					if (QMetaType::isRegistered(i) && i != QMetaType::Void)
						types[QMetaType::typeName(i)] = static_cast<QMetaType::Type>(i);
				auto property = propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), "Type");

				property->setAttribute("enumNames", QStringList(types.keys()));
				property->setValue(types.keys().indexOf(QMetaType::typeName(type)));
				propertySubGroup->addSubProperty(property);
				(*propertySlot)[property] = [this, propertyGroup, propertySubGroup, types](const QVariant &value) {
					modifyParameterType(propertyGroup->subProperties().indexOf(propertySubGroup) - 1, types.values()[value.toInt()]);
				};
			}
			{
				auto property = propertyManager->addProperty(QVariant::String, "Name");

				property->setValue(name);
				propertySubGroup->addSubProperty(property);
				(*propertySlot)[property] = [this, propertyGroup, propertySubGroup, property](const QVariant &value) {
					if (!modifyParameterName(propertyGroup->subProperties().indexOf(propertySubGroup) - 1, value.toString()))
						property->setValue(_parameters[propertyGroup->subProperties().indexOf(propertySubGroup) - 1].second);
				};
			}
			propertyGroup->addSubProperty(propertySubGroup);
		};
		{
			auto property = propertyManager->addProperty(QVariant::Int, "Count");

			property->setAttribute("minimum", 0);
			property->setValue(_parameters.count());
			propertyGroup->addSubProperty(property);
			(*propertySlot)[property] = [this, propertyGroup, addParameter](const QVariant &value) {
				auto newSize = value.toInt();

				if (newSize > _parameters.size()) {
					QString parameterName = "variable";
					QStringList parametersName;
					for (auto &parameter : _parameters)
						parametersName.append(parameter.second);
					if (parametersName.contains(parameterName)) {
						int tmp = 1;
				
						while (parametersName.contains(parameterName + "_" + QString::number(tmp)))
							tmp += 1;
						parameterName = parameterName + "_" + QString::number(tmp);
					}
					addParameter(QMetaType::Int, parameterName);
				} else if (newSize < _parameters.size()) {
					removeParameter();
					if (propertyGroup->subProperties().size() > 1)
						propertyGroup->removeSubProperty(propertyGroup->subProperties().last());
				}
			};
		}
		for (auto &parameter : _parameters)
			addParameter(parameter.first, parameter.second);
		propertyEditor->addProperty(propertyGroup);
	}
	{
		auto property = propertyManager->addProperty(QVariant::Bool, "Is const");

		property->setValue(_isConst);
		propertyEditor->addProperty(property);
		(*propertySlot)[property] = [this](const QVariant &value) {
			setConst(value.toBool());
		};
	}

	connect(propertyManager, &QtVariantPropertyManager::valueChanged, [propertySlot](QtProperty *property, const QVariant &value) {
		if (propertySlot->contains(property))
			(*propertySlot)[property](value);
	});
	connect(propertyManager, &QObject::destroyed, [propertySlot]() {
		delete propertySlot;
	});
	return propertyEditor;
}

bool types::Method::isValid() const
{
	if (_name.isEmpty()) { return false; }
	if (_isStatic && _isConst) { return false; }

	QStringList parametersName;
	for (auto &parameter : _parameters) {
		if (!parameter.second.isEmpty()) {
			if (parametersName.contains(parameter.second)) { return false; }

			parametersName.append(parameter.second);
		}
	}
	return true;
}

QString types::Method::signature() const
{
	if (!isValid()) { return ""; }
	QString signature;

	if (_isStatic)
		signature += "static ";
	signature += QString(QMetaType::typeName(_returnType)) + " " + _name + "(";
	for (int i = 0; i < _parameters.size(); i += 1) {
		if (i)
			signature += ", ";
		signature += QString(QMetaType::typeName(_parameters[i].first));
		if (!_parameters[i].second.isEmpty())
			signature += " " + _parameters[i].second;
	}
	signature += ")";
	if (_isConst)
		signature += " const";
	return signature;
}

bool types::Method::addParameter(QMetaType::Type parameterType, const QString &parameterName)
{
	for (auto &parameter : _parameters)
		if (parameter.second == parameterName) { return false; }

	_parameters.append({parameterType, parameterName});
	emit parametersChanged(_parameters);
	return true;
}

bool types::Method::modifyParameterType(int index, QMetaType::Type parameterType)
{
	if (index < 0 || index >= _parameters.size()) { return false; }

	_parameters[index].first = parameterType;
	emit parametersChanged(_parameters);
	return true;
}

bool types::Method::modifyParameterName(int index, const QString &parameterName)
{
	if (index < 0 || index >= _parameters.size()) { return false; }
	for (auto &parameter : _parameters)
		if (parameter.second == parameterName && _parameters.indexOf(parameter) != index) { return false; }

	_parameters[index].second = parameterName;
	emit parametersChanged(_parameters);
	return true;
}

void types::Method::removeParameter()
{
	if (_parameters.size() == 0) { return; }

	_parameters.removeLast();
	emit parametersChanged(_parameters);
}

QDebug operator<<(QDebug debug, const types::Method &method)
{
	debug.nospace().noquote() << "Method(";
	if (method.isValid()) {
		debug << types::accessToString(method.access()).toLower() << " ";
		debug << method.signature();
	} else
		debug << "INVALID";
	debug << ")";
	return debug.maybeSpace().maybeQuote();
}

QDebug operator<<(QDebug debug, const types::Method *method)
{
	return debug << *method;
}