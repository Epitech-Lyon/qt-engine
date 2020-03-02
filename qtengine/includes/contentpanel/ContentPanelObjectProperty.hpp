/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelObjectProperty
*/

#pragma once

#include "ContentPanelBase.hpp"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

namespace libraryObjects {
	class AObject;
}

namespace qtengine {
	class ContentPanelObjectProperty : public ContentPanelBase {
	public:
		ContentPanelObjectProperty(QWidget *parent = nullptr);
		~ContentPanelObjectProperty() = default;

		void init() override;

	private slots:
		void onCurrentObjectChanged(libraryObjects::AObject *object);
		void onValueChanged(QtProperty *property, const QVariant &value);
		void onPropertyUpdated(const QString &propertyName, const QVariant &propertyValue);

	private:
		void initObject();
		QtVariantProperty *initFlagProperty(const QString &propertyName, const QStringList &flagNames);
		QtVariantProperty *initEnumProperty(const QString &propertyName, const QStringList &enumNames);
		void refreshObject();
		QtVariantPropertyManager *_propertyManager;
		QtVariantEditorFactory *_propertyFactory;
		QtTreePropertyBrowser *_propertyEditor;
		bool _init;
		libraryObjects::AObject *_currentObject;
	};
}
