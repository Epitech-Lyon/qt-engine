/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Method
*/

#include "moc_Method.cpp"
#include "Method.hpp"

#include "Utils.hpp"
#include <QtCore/QDebug>

types::Method::Method()
	: _access(QMetaMethod::Access::Public)
	, _isStatic(false)
	, _returnType(QMetaType::Type::Void)
	, _isConst(false)
{
}

types::Method::Method(const QMetaMethod &metaMethod)
	: _access(metaMethod.access())
	, _isStatic(false)
	, _returnType(static_cast<QMetaType::Type>(metaMethod.returnType()))
	, _name(metaMethod.name())
	, _isConst(false)
{
	for (auto parameterName : metaMethod.parameterNames())
		addParameter(_parameters.size(), static_cast<QMetaType::Type>(metaMethod.parameterType(_parameters.size())), parameterName);
}

types::Method::Method(const Method &method)
	: IType()
	, _access(method.access())
	, _isStatic(method.isStatic())
	, _returnType(method.returnType())
	, _name(method.name())
	, _parameters(method.parameters())
	, _isConst(method.isConst())
{
}

types::Method &types::Method::operator=(const Method &method)
{
	if (method.isValid()) {
		setAccess(method.access());
		setStatic(method.isStatic());
		setReturnType(method.returnType());
		setName(method.name());
		setParameters(method.parameters());
		setConst(method.isConst());
	}
	return *this;
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

		addParameter(_parameters.size(), static_cast<QMetaType::Type>(jsonParameter["type"].toInt()), jsonParameter["name"].toString());
	}
	_isConst = json["isConst"].toBool();
}

bool types::Method::addParameter(int index, QMetaType::Type parameterType, const QString &parameterName)
{
	QPair<QMetaType::Type, QString> parameter(parameterType, parameterName);
	if (_parameters.contains(parameter) || index < 0 || index > _parameters.size()) { return false; }

	_parameters.append(parameter);
	return true;
}

bool types::Method::modifyParameter(int index, QMetaType::Type parameterType, const QString &parameterName)
{
	QPair<QMetaType::Type, QString> parameter(parameterType, parameterName);
	if (_parameters.contains(parameter) || index < 0 || index >= _parameters.size()) { return false; }

	_parameters[index] = parameter;
	return true;
}

void types::Method::removeParameter(int index)
{
	if (index < 0 || index >= _parameters.size()) { return; }

	_parameters.removeAt(index);
}

QDebug operator<<(QDebug debug, const types::Method &method)
{
	debug.nospace().noquote() << "Method(";
	if (method.isValid()) {
		debug << types::accessToString(method.access()).toLower();
		if (method.isStatic())
			debug << " static";
		debug << " " << QMetaType::typeName(method.returnType()) << " " << method.name() << "(";

		auto parameters = method.parameters();
		for (int i = 0; i < parameters.size(); i += 1) {
			if (i)
				debug << ", ";
			debug << QMetaType::typeName(parameters[i].first) << " " << parameters[i].second;
		}
		debug << ")";

		if (method.isConst())
			debug << " const";
	} else
		debug << "INVALID";
	debug << ")";
	return debug;
}
