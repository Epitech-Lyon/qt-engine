/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EFontComboBox
*/

#include "EFontComboBox.hpp"
#include "EComboBox.hpp"
#include "EWidget.hpp"
#include "EObject.hpp"

template<> void libraryObjects::EFontComboBox::init(AObject *object)
{
	EComboBox::init(object);
}

template<> QString libraryObjects::EFontComboBox::classIncludePath()
{
	return "#include <QtWidgets/QFontComboBox>";
}

template<> QIcon libraryObjects::EFontComboBox::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EFontComboBox::libraryFunction()
{
	return EObject::libraryFunction();
}

template<> QString libraryObjects::EFontComboBox::code(AObject *object)
{
	return EWidget::code(object);
}
