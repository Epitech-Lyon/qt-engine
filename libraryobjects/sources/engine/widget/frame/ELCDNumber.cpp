/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ELCDNumber
*/

#include "ELCDNumber.hpp"
#include "EWidget.hpp"
#include "EObject.hpp"

template<> void libraryObjects::ELCDNumber::init(AObject *object)
{
	EWidget::init(object);
}

template<> QString libraryObjects::ELCDNumber::classIncludePath()
{
	return "#include <QtWidgets/QLCDNumber>";
}

template<> QIcon libraryObjects::ELCDNumber::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ELCDNumber::libraryFunction()
{
	return EObject::libraryFunction();
}
