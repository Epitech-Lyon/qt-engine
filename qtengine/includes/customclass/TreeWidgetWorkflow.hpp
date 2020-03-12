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
		void onAddConstructorClicked();
		void onAddMethodClicked();
		void onAddSignalClicked();
		void onAddSlotClicked();
		void onAddPropertyClicked();

	private:
		libraryObjects::ObjectClass *_viewObjectClass;
		QMap<QMetaMethod::Access, QTreeWidgetItem *> _itemsConstructor;
		QMap<QMetaMethod::Access, QTreeWidgetItem *> _itemsMethod;
		QMap<QMetaMethod::Access, QTreeWidgetItem *> _itemsSignal;
		QMap<QMetaMethod::Access, QTreeWidgetItem *> _itemsSlot;
		QMap<QMetaMethod::Access, QTreeWidgetItem *> _itemsProperty;
	};
}
