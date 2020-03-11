/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Constructor
*/

#include "moc_Constructor.cpp"
#include "Constructor.hpp"

#include "Utils.hpp"
#include <QtCore/QDebug>

types::Constructor::Constructor()
	: _access(QMetaMethod::Access::Public)
{
}

types::Constructor::Constructor(const QMetaMethod &metaMethod)
	: _access(metaMethod.access())
	, _className(metaMethod.name())
{
	for (auto parameterName : metaMethod.parameterNames())
		addParameter(static_cast<QMetaType::Type>(metaMethod.parameterType(_parameters.size())), parameterName);
}

types::Constructor::Constructor(const Constructor &constructor)
	: IType()
	, _access(constructor.access())
	, _className(constructor.className())
	, _parameters(constructor.parameters())
{
}

types::Constructor &types::Constructor::operator=(const Constructor &constructor)
{
	if (constructor.isValid()) {
		setAccess(constructor.access());
		setClassName(constructor.className());
		setParameters(constructor.parameters());
	}
	return *this;
}

bool types::Constructor::operator==(const Constructor &constructor)
{
	return _className == constructor.className() && _parameters == constructor.parameters();
}

QJsonObject types::Constructor::serialize() const
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
	json["className"] = _className;
	json["parameters"] = jsonParameters;
	return json;
}

void types::Constructor::deserialize(const QJsonObject &json)
{
	_access = static_cast<QMetaMethod::Access>(json["access"].toInt());
	_className = json["className"].toString();
	for (auto jsonParameterRef : json["parameters"].toArray()) {
		auto jsonParameter = jsonParameterRef.toObject();

		addParameter(static_cast<QMetaType::Type>(jsonParameter["type"].toInt()), jsonParameter["name"].toString());
	}
}

bool types::Constructor::isValid() const
{
	if (_className.isEmpty()) { return false; }

	QStringList parametersName;
	for (auto &parameter : _parameters) {
		if (parameter.second.isEmpty() || parametersName.contains(parameter.second)) { return false; }

		parametersName.append(parameter.second);
	}
	return true;
}

QString types::Constructor::signature() const
{
	if (!isValid()) { return ""; }
	QString signature;
	
	signature += _className + "::" + _className + "(";
	for (int i = 0; i < _parameters.size(); i += 1) {
		if (i)
			signature += ", ";
		signature += QString(QMetaType::typeName(_parameters[i].first)) + " " + _parameters[i].second;
	}
	signature += ")";
	return signature;
}

bool types::Constructor::addParameter(QMetaType::Type parameterType, const QString &parameterName)
{
	return insertParameter(_parameters.size(), parameterType, parameterName);
}

bool types::Constructor::insertParameter(int index, QMetaType::Type parameterType, const QString &parameterName)
{
	if (index < 0 || index > _parameters.size()) { return false; }
	for (auto &parameter : _parameters)
		if (parameter.second == parameterName) { return false; }

	_parameters.append({parameterType, parameterName});
	return true;
}

bool types::Constructor::modifyParameterType(int index, QMetaType::Type parameterType)
{
	if (index < 0 || index >= _parameters.size()) { return false; }

	_parameters[index].first = parameterType;
	return true;
}

bool types::Constructor::modifyParameterName(int index, const QString &parameterName)
{
	if (index < 0 || index >= _parameters.size()) { return false; }
	for (auto &parameter : _parameters)
		if (parameter.second == parameterName && _parameters.indexOf(parameter) != index) { return false; }

	_parameters[index].second = parameterName;
	return true;
}

void types::Constructor::removeParameter(int index)
{
	if (index < 0 || index >= _parameters.size()) { return; }

	_parameters.removeAt(index);
}

QDebug operator<<(QDebug debug, const types::Constructor &constructor)
{
	debug.nospace().noquote() << "Constructor(";
	if (constructor.isValid()) {
		debug << types::accessToString(constructor.access()).toLower() << " ";
		debug << constructor.signature();
	} else
		debug << "INVALID";
	debug << ")";
	return debug.maybeSpace().maybeQuote();
}

QDebug operator<<(QDebug debug, const types::Constructor *constructor)
{
	return debug << *constructor;
}
