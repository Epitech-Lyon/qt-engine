/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelBase
*/

#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include "ISerializable.hpp"

namespace qtengine {
	class LayoutPanelBase : public QWidget, public ISerializable {
	public:
		LayoutPanelBase(QWidget * = nullptr);
		virtual ~LayoutPanelBase() = default;

		virtual QJsonObject serialize() const override { return QJsonObject(); }
		virtual void deserialize(const QJsonObject &) override {}

		virtual LayoutPanelBase *parentLayoutPanel();

		LayoutPanelBase *child() const { return _child; }
		void setChild(LayoutPanelBase *);

	protected:
		QVBoxLayout *_mainLayout;
		LayoutPanelBase *_child;
	};
}
