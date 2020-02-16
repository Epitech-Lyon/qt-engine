/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelLibraryObject
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtWidgets/QTreeWidget>
#include <QtCore/QMap>

namespace libraryObjects {
	class LibraryObject;
}

namespace qtengine {
	class TreeLibraryObject : public QTreeWidget {
	public:
		TreeLibraryObject(QWidget *parent = nullptr);
		~TreeLibraryObject() = default;

		void addLibraryObject(libraryObjects::LibraryObject *libraryObject);

	private:
		QTreeWidgetItem *addItem(QTreeWidgetItem *parent, const QString &classHierarchy);
		QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const override;
		QMap<QTreeWidgetItem *, libraryObjects::LibraryObject *> _libraryObjects;
	};

	class ContentPanelLibraryObject : public ContentPanelBase {
	public:
		ContentPanelLibraryObject(QWidget *parent = nullptr);
		~ContentPanelLibraryObject() = default;

		void init() override;

	private:
		TreeLibraryObject *_tree;
	};
}
