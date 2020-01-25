/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelBase
*/

#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include "ISerializable.hpp"

namespace qtengine {
	class ContentPanelBase : public QWidget, public ISerializable {
	public:
		virtual ~ContentPanelBase() = default;

		virtual void init();

		virtual QJsonObject serialize() const override;
		virtual void deserialize(const QJsonObject &) override;

		QString name() const { return _name; }

	protected:
		ContentPanelBase(const QString &, QWidget * = nullptr);
		virtual QToolBar *initToolBar() { return nullptr; }

		QVBoxLayout *_mainLayout;

	private:
		QString _name;
	};
}
