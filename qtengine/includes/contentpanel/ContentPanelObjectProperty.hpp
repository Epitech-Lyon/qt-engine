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
		void onValueChanged(QtProperty *, const QVariant &);

	private:
		QtVariantPropertyManager *_propertyManager;
		QtVariantEditorFactory *_propertyFactory;
		QtTreePropertyBrowser *_propertyEditor;
	};
}
