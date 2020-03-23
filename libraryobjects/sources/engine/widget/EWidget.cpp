/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EWidget
*/

#include "EWidget.hpp"
#include "EObject.hpp"

#include "LibraryFunction.hpp"
#include "LibraryObjectManager.hpp"
#include "ObjectManager.hpp"

#include <QtWidgets/QLayout>

template<> void libraryObjects::EWidget::init(AObject *object)
{
	auto widget = dynamic_cast<QWidget*>(object->object());

	EObject::init(object);
	connect(widget, &QWidget::windowIconChanged, object, [object](const QIcon &icon) { emit object->propertyUpdated("windowIcon", icon); });
	connect(widget, &QWidget::windowTitleChanged, object, [object](const QString &title) { emit object->propertyUpdated("windowTitle", title); });
}

template<> QIcon libraryObjects::EWidget::icon()
{
	return QIcon();
}

template<> QString libraryObjects::EWidget::classIncludePath()
{
	return "#include <QtWidgets/QWidget>";
}

template<> libraryObjects::LibraryFunction *libraryObjects::EWidget::libraryFunction()
{
	auto libraryFunction = EObject::libraryFunction();

	libraryFunction->addFunctionDrag(Object<QLayout>::classHierarchy(), LibraryFunction::FunctionDrag("setLayout", Widget::setLayout, "unsetLayout", Widget::unsetLayout));
	return libraryFunction;
}

template<> QString libraryObjects::EWidget::code(AObject *object)
{
	QString code = EObject::code(object);

	for (AObject *objectParent = object->parent(); objectParent; objectParent = objectParent->parent())
		if (LibraryObjectManager::isSubClassOf(objectParent->classHierarchy(), Object<QWidget>::classHierarchy())) {
			code += ObjectManager::instance()->objectName(object->id()) + "->setParent(" + ObjectManager::instance()->objectName(objectParent->id()) + ");\n";
			break;
		}
	if (object->children().size() == 1 && LibraryObjectManager::isSubClassOf(object->children().front()->classHierarchy(), Object<QLayout>::classHierarchy()))
		code += ObjectManager::instance()->objectName(object->id()) + "->setLayout(" + ObjectManager::instance()->objectName(object->children().front()->id()) + ");\n";
	return code;
}

bool libraryObjects::Widget::setLayout(AObject *parent, int, AObject *child)
{
	auto widget = dynamic_cast<QWidget*>(parent->object());
	if (!widget || widget->layout()) { return false; }

	auto layout = dynamic_cast<QLayout*>(child->object());
	if (!layout) { return false; }

	widget->setLayout(layout);
	parent->addChild(child);
	return true;
}

bool libraryObjects::Widget::unsetLayout(AObject *parent, AObject *child)
{
	auto widget = dynamic_cast<QWidget*>(parent->object());
	if (!widget) { return false; }

	auto layout = dynamic_cast<QLayout*>(child->object());
	if (!layout) { return false; }

	parent->removeChild(child);
	return true;
}
