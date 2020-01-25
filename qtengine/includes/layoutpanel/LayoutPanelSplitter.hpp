/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelSplitter
*/

#pragma once

#include "LayoutPanelBase.hpp"
#include <QtWidgets/QSplitter>
#include <QtCore/QVector>

namespace qtengine {
	class LayoutPanelTabber;
	class LayoutPanelSplitter : public LayoutPanelBase {
	public:
		LayoutPanelSplitter(LayoutPanelTabber *, Qt::Orientation, QWidget * = nullptr);
		LayoutPanelSplitter(Qt::Orientation, QWidget * = nullptr);
		~LayoutPanelSplitter() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

		void removeTabber(LayoutPanelTabber *);

		void split(Qt::Orientation, LayoutPanelTabber *);

	private:
		void unsplit(LayoutPanelBase *, LayoutPanelSplitter *);
		QVector<QWidget *> widgets() const;
		QSplitter *_splitter;
	};
}
