/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ELineEdit
*/

#include "ELineEdit.hpp"
#include "EObject.hpp"

template<> void libraryObjects::ELineEdit::init(AObject *object)
{
	auto lineEdit = dynamic_cast<QLineEdit*>(object->object());

	EObject::init(object);
	connect(lineEdit, &QLineEdit::textChanged, object, [object](const QString &text) { emit object->propertyUpdated("text", text); });
}

template<> QString libraryObjects::ELineEdit::classIncludePath()
{
	return "#include <QtWidgets/QLineEdit>";
}

template<> QIcon libraryObjects::ELineEdit::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ELineEdit::libraryFunction()
{
	return EObject::libraryFunction();
}

template<> QString libraryObjects::ELineEdit::code(AObject *object)
{
	return EObject::code(object);
}
