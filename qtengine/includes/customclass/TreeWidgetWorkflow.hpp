/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TreeWidgetWorkflow
*/

#pragma once

#include <QtWidgets/QTreeWidget>
#include <QtCore/QMetaMethod>
#include <QtCore/QMap>

#include <QtCore/QDebug>
#include <QtCore/QVariant>

#include "ClassType.hpp"

namespace libraryObjects {
	class ObjectClass;
}

namespace qtengine {
	class TreeWidgetWorkflow : public QTreeWidget {
	public:
		TreeWidgetWorkflow(QWidget *parent = nullptr);
		~TreeWidgetWorkflow() = default;

		void clear();

	public slots:
		void onViewObjectClassChanged(libraryObjects::ObjectClass *objectClass);

	private slots:
		void onCustomContextMenuRequested(const QPoint &pos);

	private:
		void addTypeItem(types::ClassType *classType);
		void onAddClicked(types::ClassType *classType);
		void onSettingsClicked(QTreeWidgetItem *item);
		void onDeleteClicked(QTreeWidgetItem *item);

		libraryObjects::ObjectClass *_viewObjectClass;
		QMap<types::ClassType::Type, QMap<QMetaMethod::Access, QTreeWidgetItem *>> _items;
		QMap<QTreeWidgetItem *, types::ClassType *> _childItems;
	};
}
