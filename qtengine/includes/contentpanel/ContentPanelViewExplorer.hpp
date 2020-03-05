/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelViewExplorer
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtWidgets/QTreeWidget>

namespace libraryObjects {
	class AObject;
	class LibraryObject;
}

namespace qtengine {
	class TreeViewExplorer : public QTreeWidget {
		Q_OBJECT

	public:
		TreeViewExplorer(QWidget *parent = nullptr);
		~TreeViewExplorer() = default;

		QTreeWidgetItem *createItemFor(libraryObjects::AObject *object, QTreeWidgetItem *parent, bool recursively = true, int index = -1);
		QTreeWidgetItem *createItemFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, bool recursively = true, int index = -1);
		void removeItemFor(libraryObjects::AObject *object);

	public slots:
		void setCurrentItemFor(libraryObjects::AObject *object);

	signals:
		void objectClicked(libraryObjects::AObject *object);
		void openMenuFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, const QPoint &pos);
		void libraryObjectDropped(libraryObjects::AObject *parent, int index, libraryObjects::LibraryObject *libraryObject);

	private slots:
		void onPropertyUpdated(const QString &propertyName, const QVariant &propertyValue);

	private:
		void dragMoveEvent(QDragMoveEvent *event);
		bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action) override;
		QMap<QTreeWidgetItem *, libraryObjects::AObject *> _objects;
		QTreeWidgetItem *_itemEditorOpened;
	};

	class ContentPanelViewExplorer : public ContentPanelBase {
		Q_OBJECT

	public:
		ContentPanelViewExplorer(QWidget * = nullptr);
		~ContentPanelViewExplorer() = default;

		void init() override;

	private slots:
		void onViewObjectChanged(libraryObjects::AObject *viewObject);
		void onOpenMenuFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, const QPoint &pos);
		void onLibraryObjectDropped(libraryObjects::AObject *parent, int index, libraryObjects::LibraryObject *libraryObject);

	private:
		TreeViewExplorer *_tree;
	};
};
