/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ERadioButton
*/

#include "ERadioButton.hpp"
#include "EAbstractButton.hpp"

template<> void libraryObjects::ERadioButton::init(AObject *object)
{
	EAbstractButton::init(object);
}

template<> QString libraryObjects::ERadioButton::classIncludePath()
{
	return "#include <QtWidgets/QRadioButton>";
}

template<> QIcon libraryObjects::ERadioButton::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ERadioButton::libraryFunction()
{
	return EAbstractButton::libraryFunction();
}

template<> QString libraryObjects::ERadioButton::code(AObject *object)
{
	return EAbstractButton::code(object);
}
