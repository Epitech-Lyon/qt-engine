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

namespace QtNodes {
	class FlowView;
	class DataModelRegistry;
}

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
		void onObjectClassDropped(const QPointF &pos, libraryObjects::ObjectClass *objectClass, libraryObjects::AObject *reference);

	private:
		std::shared_ptr<QtNodes::DataModelRegistry> generateRegistryBuiltIn() const;
		std::shared_ptr<QtNodes::DataModelRegistry> generateRegistryView(const QMetaObject *metaObject, QMetaMethod::Access minimumAccess) const;

		QtNodes::FlowView *_view;
		FlowSceneWorkflow *_scene;
		std::shared_ptr<QtNodes::DataModelRegistry> _registryBuiltIn;

		TreeWidgetWorkflow *_tree;
	};
}
