/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewProperty
*/

#pragma once

#include "ContentPanelBase.hpp"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

namespace qtengine {
    class ContentPanelViewProperty : public ContentPanelBase {
    public:
        ContentPanelViewProperty(QWidget * = nullptr);
        ~ContentPanelViewProperty() = default;

    void init() override;

    private:
		QtVariantPropertyManager *_propertyManager;
		QtVariantEditorFactory *_propertyFactory;
		QtTreePropertyBrowser *_propertyEditor;
    };
}
