/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Signal
*/

#include "moc_Signal.cpp"
#include "Signal.hpp"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qtgroupboxpropertybrowser.h"

#include "Utils.hpp"
#include <QtCore/QDebug>

types::Signal::Signal()
	: ClassType(QMetaMethod::Access::Public, Type::SIGNAL)
{
}

types::Signal::Signal(const QMetaMethod &metaMethod)
	: ClassType(metaMethod.access(), Type::SIGNAL)
	, _name(metaMethod.name())
{
	for (auto parameterName : metaMethod.parameterNames())
		addParameter(static_cast<QMetaType::Type>(metaMethod.parameterType(_parameters.size())), parameterName);
}

QJsonObject types::Signal::serialize() const
{
	QJsonArray jsonParameters;
	for (const auto &parameter : _parameters) {
		QJsonObject jsonParameter;
		jsonParameter["type"] = static_cast<int>(parameter.first);
		jsonParameter["name"] = parameter.first;
		jsonParameters.append(jsonParameter);
	}

	QJsonObject json;
	json["name"] = _name;
	json["parameters"] = jsonParameters;
	return json;
}

void types::Signal::deserialize(const QJsonObject &json)
{
	_name = json["name"].toString();
	for (auto jsonParameterRef : json["parameters"].toArray()) {
		auto jsonParameter = jsonParameterRef.toObject();

		addParameter(static_cast<QMetaType::Type>(jsonParameter["type"].toInt()), jsonParameter["name"].toString());
	}
}

QWidget *types::Signal::initEditor()
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

	connect(propertyManager, &QtVariantPropertyManager::valueChanged, [propertySlot](QtProperty *property, const QVariant &value) {
		if (propertySlot->contains(property))
			(*propertySlot)[property](value);
	});
	connect(propertyManager, &QObject::destroyed, [propertySlot]() {
		delete propertySlot;
	});
	return propertyEditor;
}

bool types::Signal::isValid() const
{
	if (_name.isEmpty()) { return false; }

	QStringList parametersName;
	for (auto &parameter : _parameters) {
		if (!parameter.second.isEmpty()) {
			if (parametersName.contains(parameter.second)) { return false; }

			parametersName.append(parameter.second);
		}
	}
	return true;
}

QString types::Signal::signature() const
{
	if (!isValid()) { return ""; }
	QString signature;

	signature += QString(QMetaType::typeName(QMetaType::Void)) + " " + _name + "(";
	for (int i = 0; i < _parameters.size(); i += 1) {
		if (i)
			signature += ", ";
		signature += QString(QMetaType::typeName(_parameters[i].first));
		if (!_parameters[i].second.isEmpty())
			signature += " " + _parameters[i].second;
	}
	signature += ")";
	return signature;
}

bool types::Signal::addParameter(QMetaType::Type parameterType, const QString &parameterName)
{
	for (auto &parameter : _parameters)
		if (parameter.second == parameterName) { return false; }

	_parameters.append({parameterType, parameterName});
	emit parametersChanged(_parameters);
	return true;
}

bool types::Signal::modifyParameterType(int index, QMetaType::Type parameterType)
{
	if (index < 0 || index >= _parameters.size()) { return false; }

	_parameters[index].first = parameterType;
	emit parametersChanged(_parameters);
	return true;
}

bool types::Signal::modifyParameterName(int index, const QString &parameterName)
{
	if (index < 0 || index >= _parameters.size()) { return false; }
	for (auto &parameter : _parameters)
		if (parameter.second == parameterName && _parameters.indexOf(parameter) != index) { return false; }

	_parameters[index].second = parameterName;
	emit parametersChanged(_parameters);
	return true;
}

void types::Signal::removeParameter()
{
	if (_parameters.size() == 0) { return; }

	_parameters.removeLast();
	emit parametersChanged(_parameters);
}

QDebug operator<<(QDebug debug, const types::Signal &signal)
{
	debug.nospace().noquote() << "Method(";
	if (signal.isValid()) {
		debug << types::accessToString(signal.access()).toLower() << " ";
		debug << signal.signature();
	} else
		debug << "INVALID";
	debug << ")";
	return debug.maybeSpace().maybeQuote();
}

QDebug operator<<(QDebug debug, const types::Signal *signal)
{
	return debug << *signal;
}