/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** BuiltIn
*/

#pragma once

#include "NodeDataModel.hpp"
#include <QtCore/QMetaType>

class QtGroupBoxPropertyBrowser;
class QtVariantEditorFactory;
class QtVariantPropertyManager;

namespace qtengine {
	class BuiltIn : public QtNodes::NodeDataModel {
	public:
		BuiltIn(QMetaType::Type type);
		~BuiltIn() = default;

		QString name() const override;

		QString caption() const override;
		bool captionVisible() const override { return false; }

		unsigned int nPorts(QtNodes::PortType portType) const override;
		QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

		QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override { return true; }

		QtNodes::NodeDataModel::ConnectionPolicy portOutConnectionPolicy(QtNodes::PortIndex portIndex) const override;

		QWidget *embeddedWidget() override;

	private:
		QMetaType::Type _type;
		QtGroupBoxPropertyBrowser *_propertyEditor;
		QtVariantEditorFactory *_propertyFactory;
		QtVariantPropertyManager *_propertyManager;
	};
}
