/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TreeWidgetLibraryObject
*/

#pragma once

#include <QtWidgets/QTreeWidget>
#include <QtCore/QMap>

namespace libraryObjects {
	class LibraryObject;
}

namespace qtengine {
	class TreeWidgetLibraryObject : public QTreeWidget {
	public:
		TreeWidgetLibraryObject(QWidget *parent = nullptr);
		~TreeWidgetLibraryObject() = default;

		void addLibraryObject(libraryObjects::LibraryObject *libraryObject);

	private:
		QTreeWidgetItem *addItem(QTreeWidgetItem *parent, const QString &classHierarchy);
		QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const override;
		QMap<QTreeWidgetItem *, libraryObjects::LibraryObject *> _libraryObjects;
	};
}
