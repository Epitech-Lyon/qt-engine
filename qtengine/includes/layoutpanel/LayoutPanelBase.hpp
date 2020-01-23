/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelBase
*/

#pragma once

#include <QtWidgets/QtWidgets>
#include "ISerializable.hpp"

namespace qtengine {
	class LayoutPanelBase : public QWidget, public ISerializable {
	public:
		LayoutPanelBase(QWidget * = nullptr);
		virtual ~LayoutPanelBase() = default;

		virtual QJsonObject serialize() const override { return QJsonObject(); }
		virtual void deserialize(const QJsonObject &) override {}

		virtual LayoutPanelBase *parentLayoutPanel();

		void setChildPanel(LayoutPanelBase *);

	protected:
		QVBoxLayout *_mainLayout;
	};
}
