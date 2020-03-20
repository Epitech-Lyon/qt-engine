/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassType
*/

#include "moc_ClassType.cpp"
#include "ClassType.hpp"

#include "Constructor.hpp"
#include "Method.hpp"
#include "Signal.hpp"
#include "Slot.hpp"
#include "Property.hpp"

types::ClassType *types::ClassType::construct(Type type)
{
	if (type == Type::CONSTRUCTOR)
		return new Constructor();
	if (type == Type::METHOD)
		return new Method();
	if (type == Type::SIGNAL)
		return new Signal();
	if (type == Type::SLOT)
		return new Slot();
	if (type == Type::PROPERTY)
		return new Property();
	return nullptr;
}

types::ClassType::ClassType(QMetaMethod::Access access, Type type)
	: _access(access)
	, _type(type)
{
}

QJsonObject types::ClassType::serialize() const
{
	QJsonObject json;

	json["type"] = static_cast<int>(_type);
	json["content"] = _content;
	json["access"] = static_cast<int>(_access);
	return json;
}

void types::ClassType::deserialize(const QJsonObject &json)
{
	_content = json["content"].toObject();
	_access = static_cast<QMetaMethod::Access>(json["access"].toInt());
}
