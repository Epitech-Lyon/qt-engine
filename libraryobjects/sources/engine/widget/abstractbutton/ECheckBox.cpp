/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ECheckBox
*/

#include "ECheckBox.hpp"
#include "EAbstractButton.hpp"

template<> void libraryObjects::ECheckBox::init(AObject *object)
{
	EAbstractButton::init(object);
}

template<> QString libraryObjects::ECheckBox::classIncludePath()
{
	return "#include <QtWidgets/QCheckBox>";
}

template<> QIcon libraryObjects::ECheckBox::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ECheckBox::libraryFunction()
{
	return EAbstractButton::libraryFunction();
}
