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
		addParameter(static_cast<QMetaType::Type>(metaMethod.parameterType(_parameters.size())), parameterName);
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

bool types::Method::operator==(const Method &method)
{
	return _returnType == method.returnType() && _name == method.name() && _parameters == method.parameters() && _isConst == method.isConst();
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

bool types::Method::isValid() const
{
	if (_name.isEmpty()) { return false; }
	if (_isStatic && _isConst) { return false; }

	QStringList parametersName;
	for (auto &parameter : _parameters) {
		if (parameter.second.isEmpty() || parametersName.contains(parameter.second)) { return false; }

		parametersName.append(parameter.second);
	}
	return true;
}

QString types::Method::signature() const
{
	if (!isValid()) { return ""; }
	QString signature;

	if (_isStatic)
		signature += " static";
	signature += " " + QString(QMetaType::typeName(_returnType)) + " " + _name + "(";
	for (int i = 0; i < _parameters.size(); i += 1) {
		if (i)
			signature += ", ";
		signature += QString(QMetaType::typeName(_parameters[i].first)) + " " + _parameters[i].second;
	}
	signature += ")";
	if (_isConst)
		signature += " const";
	return signature;
}

bool types::Method::addParameter(QMetaType::Type parameterType, const QString &parameterName)
{
	return insertParameter(_parameters.size(), parameterType, parameterName);
}

bool types::Method::insertParameter(int index, QMetaType::Type parameterType, const QString &parameterName)
{
	if (index < 0 || index > _parameters.size()) { return false; }
	for (auto &parameter : _parameters)
		if (parameter.second == parameterName) { return false; }

	_parameters.append({parameterType, parameterName});
	return true;
}

bool types::Method::modifyParameterType(int index, QMetaType::Type parameterType)
{
	if (index < 0 || index >= _parameters.size()) { return false; }

	_parameters[index].first = parameterType;
	return true;
}

bool types::Method::modifyParameterName(int index, const QString &parameterName)
{
	if (index < 0 || index >= _parameters.size()) { return false; }
	for (auto &parameter : _parameters)
		if (parameter.second == parameterName && _parameters.indexOf(parameter) != index) { return false; }

	_parameters[index].second = parameterName;
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