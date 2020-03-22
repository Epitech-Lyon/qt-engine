/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ETextEdit
*/

#include "ETextEdit.hpp"
#include "EAbstractScrollArea.hpp"

template<> void libraryObjects::ETextEdit::init(AObject *object)
{
	auto text = dynamic_cast<QTextEdit*>(object->object());

	EAbstractScrollArea::init(object);
	connect(text, &QTextEdit::textChanged, object, [object, text]()
	{
		emit object->propertyUpdated("markdown", text->toMarkdown());
		emit object->propertyUpdated("html", text->toHtml());
	});
}

template<> QString libraryObjects::ETextEdit::classIncludePath()
{
	return "#include <QtWidgets/QTextEdit>";
}

template<> QIcon libraryObjects::ETextEdit::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::ETextEdit::libraryFunction()
{
	return EAbstractScrollArea::libraryFunction();
}
