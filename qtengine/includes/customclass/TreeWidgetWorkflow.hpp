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

namespace libraryObjects {
	class ObjectClass;
}

namespace types {
	class Constructor;
	class Method;
	class Property;
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
		void onAddConstructorClicked();
		void onAddMethodClicked();
		void onAddSignalClicked();
		void onAddSlotClicked();
		void onAddPropertyClicked();
		void onSettingsConstructorClicked(QTreeWidgetItem *item, types::Constructor *constructor);
		void onSettingsMethodClicked(QTreeWidgetItem *item, types::Method *method);
		void onSettingsSignalClicked(QTreeWidgetItem *item, types::Method *signal);
		void onSettingsSlotClicked(QTreeWidgetItem *item, types::Method *slot);
		void onSettingsPropertyClicked(QTreeWidgetItem *item, types::Property *property);
		void onDeleteConstructorClicked(QTreeWidgetItem *item, types::Constructor *constructor);
		void onDeleteMethodClicked(QTreeWidgetItem *item, types::Method *method);
		void onDeleteSignalClicked(QTreeWidgetItem *item, types::Method *signal);
		void onDeleteSlotClicked(QTreeWidgetItem *item, types::Method *slot);
		void onDeletePropertyClicked(QTreeWidgetItem *item, types::Property *property);

	private:
		using ItemsType = QMap<QMetaMethod::Access, QTreeWidgetItem *>;
		template <typename IType> using ChildItemsType = QMap<QTreeWidgetItem *, IType *>;

		template <typename IType> void addTypeItem(ItemsType &itemsType, ChildItemsType<IType> &childItemsType, IType *type)
		{
			auto parentItem = itemsType[type->access()];
			auto item = new QTreeWidgetItem(parentItem, { type->signature() });

			setCurrentItem(item);
			childItemsType[item] = type;
		}

		libraryObjects::ObjectClass *_viewObjectClass;
		ItemsType _itemsConstructor;
		ChildItemsType<types::Constructor> _childItemsConstructor;
		ItemsType _itemsMethod;
		ChildItemsType<types::Method> _childItemsMethod;
		ItemsType _itemsSignal;
		ChildItemsType<types::Method> _childItemsSignal;
		ItemsType _itemsSlot;
		ChildItemsType<types::Method> _childItemsSlot;
		ItemsType _itemsProperty;
		ChildItemsType<types::Property> _childItemsProperty;
	};
}
