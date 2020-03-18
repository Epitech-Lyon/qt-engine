/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TreeWidgetViewExplorer
*/

#pragma once

#include <QtWidgets/QTreeWidget>

namespace libraryObjects {
	class AObject;
	class LibraryObject;
}

namespace qtengine {
	class TreeWidgetViewExplorer : public QTreeWidget {
		Q_OBJECT

	public:
		TreeWidgetViewExplorer(QWidget *parent = nullptr);
		~TreeWidgetViewExplorer() = default;

		QTreeWidgetItem *createItemFor(libraryObjects::AObject *object, QTreeWidgetItem *parent, bool recursively = true, int index = -1);
		QTreeWidgetItem *createItemFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, bool recursively = true, int index = -1);

		QTreeWidgetItem *itemFor(libraryObjects::AObject *object) const;
		void expandRecursivelyItemFor(libraryObjects::AObject *object) const;

		void removeItemFor(libraryObjects::AObject *object);

	public slots:
		void setCurrentItemFor(libraryObjects::AObject *object);

	signals:
		void objectClicked(libraryObjects::AObject *object);
		void openMenuFor(libraryObjects::AObject *object, libraryObjects::AObject *parent, const QPoint &pos);
		void libraryObjectDropped(libraryObjects::AObject *parent, int index, libraryObjects::LibraryObject *libraryObject, libraryObjects::AObject *reference);

	private slots:
		void onPropertyUpdated(const QString &propertyName, const QVariant &propertyValue);

	private:
		void dragEnterEvent(QDragEnterEvent *event) override;
		void dragMoveEvent(QDragMoveEvent *event) override;
		bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action) override;
		QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const override;
		QMap<QTreeWidgetItem *, libraryObjects::AObject *> _objects;
		QTreeWidgetItem *_itemEditorOpened;
	};
}
