/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewExplorer
*/

#pragma once

#include "ContentPanelBase.hpp"

namespace libraryObjects {
	class AObject;
	class LibraryObject;
}

namespace qtengine {
	class TreeWidgetViewExplorer;

	class ContentPanelViewExplorer : public ContentPanelBase {
		Q_OBJECT

	public:
		ContentPanelViewExplorer(QWidget * = nullptr);
		~ContentPanelViewExplorer() = default;

		void init() override;

	private slots:
		void onViewObjectChanged(libraryObjects::AObject *viewObject);
		void onOpenMenuFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, const QPoint &pos);
		void onLibraryObjectDropped(libraryObjects::AObject *parent, int index, libraryObjects::LibraryObject *libraryObject, libraryObjects::AObject *reference);

	private:
		TreeWidgetViewExplorer *_tree;
	};
};
