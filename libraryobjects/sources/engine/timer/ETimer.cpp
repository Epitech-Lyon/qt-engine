/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ETimer
*/

#include "ETimer.hpp"
#include "EObject.hpp"

template<> void libraryObjects::ETimer::init(AObject *object)
{
	EObject::init(object);
}

template<> QString libraryObjects::ETimer::classIncludePath()
{
	return "#include <QtCore/QTimer>";
}

template<> QIcon libraryObjects::ETimer::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ETimer::libraryFunction()
{
	return EObject::libraryFunction();
}

template<> QString libraryObjects::ETimer::code(AObject *object)
{
	return EObject::code(object);
}
