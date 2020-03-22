/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EPlainTextEdit
*/

#include "EPlainTextEdit.hpp"
#include "EAbstractScrollArea.hpp"

template<> void libraryObjects::EPlainTextEdit::init(AObject *object)
{
	auto plainText = dynamic_cast<QPlainTextEdit*>(object->object());

	EAbstractScrollArea::init(object);
	connect(plainText, &QPlainTextEdit::textChanged, object, [object, plainText]() { emit object->propertyUpdated("plainText", plainText->toPlainText()); });
}

template<> QString libraryObjects::EPlainTextEdit::classIncludePath()
{
	return "#include <QtWidgets/QPlainTextEdit>";
}

template<> QIcon libraryObjects::EPlainTextEdit::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EPlainTextEdit::libraryFunction()
{
	return EAbstractScrollArea::libraryFunction();
}
