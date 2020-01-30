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

namespace qtengine {
	class ContentPanelObjectProperty : public ContentPanelBase {
	public:
		ContentPanelObjectProperty(QWidget * = nullptr);
		~ContentPanelObjectProperty() = default;

		void init() override;

	private:
		QtVariantPropertyManager *_propertyManager;
		QtVariantEditorFactory *_propertyFactory;
		QtTreePropertyBrowser *_propertyEditor;
	};
}
