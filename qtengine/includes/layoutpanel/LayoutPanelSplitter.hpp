/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutPanelSplitter
*/

#pragma once

#include "LayoutPanelBase.hpp"
#include <QtWidgets/QSplitter>

namespace qtengine {
	class LayoutPanelSplitter : public LayoutPanelBase {
	public:
		LayoutPanelSplitter(QWidget * = nullptr);
		~LayoutPanelSplitter() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

		void setChildrenPanel(LayoutPanelBase *, LayoutPanelBase *);
		void setChildPanel(int, LayoutPanelBase *);
		void setOrientation(Qt::Orientation);
		int indexOf(LayoutPanelBase *);

		LayoutPanelBase *firstLayoutPanel() const { return _firstLayoutPanel; }
		void setFirstLayoutPanel(LayoutPanelBase *);

		LayoutPanelBase *secondLayoutPanel() const { return _secondLayoutPanel; }
		void setSecondLayoutPanel(LayoutPanelBase *);

	private:
		QSplitter *_splitter;
		LayoutPanelBase *_firstLayoutPanel;
		LayoutPanelBase *_secondLayoutPanel;

		void removeAllPanels();
	};

}
