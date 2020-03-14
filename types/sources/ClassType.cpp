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
