/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtCore/QMetaObject>
#include <QtCore/QMetaMethod>
#include <QtCore/QUuid>

namespace QtNodes {
	class FlowView;
	class DataModelRegistry;
}

namespace types {
	class ClassType;
};

namespace libraryObjects {
	class AObject;
	class ObjectClass;
}

namespace qtengine {
	class TreeWidgetWorkflow;
	class FlowSceneWorkflow;

	class ContentPanelWorkflow : public ContentPanelBase {
		Q_OBJECT

	public:
		ContentPanelWorkflow(QWidget *parent = nullptr);
		~ContentPanelWorkflow() = default;

		void init() override;

	private slots:
		void onObjectChanged(libraryObjects::AObject *viewObject);
		void onObjectClassChanged(libraryObjects::ObjectClass *objectClass);
		void onObjectClassDropped(const QPointF &pos, libraryObjects::ObjectClass *objectClass, libraryObjects::AObject *reference, QObject *source);
		void onClassTypeDoubleClicked(types::ClassType *classType);
		void onClassTypeAdded(types::ClassType *classType);
		void onClassTypeDeleted(types::ClassType *classType);
		void onSaveRequested();

	private:
		std::shared_ptr<QtNodes::DataModelRegistry> generateRegistryTypeFunction() const;
		std::shared_ptr<QtNodes::DataModelRegistry> generateRegistryObjectClass(libraryObjects::ObjectClass *objectClass, QMetaMethod::Access minimumAccess, const QUuid &objectId) const;

		TreeWidgetWorkflow *_tree;
		QtNodes::FlowView *_view;
		FlowSceneWorkflow *_scene;
		types::ClassType *_currentClassType;
	};
}
