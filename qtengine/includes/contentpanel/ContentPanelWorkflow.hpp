/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#pragma once

#include "ContentPanelBase.hpp"
#include "FlowScene.hpp"

#include <QtWidgets/QTreeWidget>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaMethod>
#include <QtCore/QMap>

namespace QtNodes {
	class FlowView;
	class DataModelRegistry;
}

namespace libraryObjects {
	class AObject;
}

namespace qtengine {
	class TreeWidgetWorkflow;

	class FlowScene : public QtNodes::FlowScene {
		Q_OBJECT

	public:
		FlowScene(QObject *parent = nullptr);
		~FlowScene() = default;

	private:
		void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
		void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
		void dropEvent(QGraphicsSceneDragDropEvent *event);
	};

	class ContentPanelWorkflow : public ContentPanelBase {
		Q_OBJECT

	public:
		ContentPanelWorkflow(QWidget *parent = nullptr);
		~ContentPanelWorkflow() = default;

		void init() override;

	private slots:
		void onViewObjectChanged(libraryObjects::AObject *viewObject);

	private:
		std::shared_ptr<QtNodes::DataModelRegistry> generateRegistryBuiltIn() const;
		std::shared_ptr<QtNodes::DataModelRegistry> generateRegistryView(const QMetaObject *metaObject, QMetaMethod::Access minimumAccess) const;

		QtNodes::FlowView *_view;
		FlowScene *_scene;
		std::shared_ptr<QtNodes::DataModelRegistry> _registryBuiltIn;

		TreeWidgetWorkflow *_tree;
	};
}
