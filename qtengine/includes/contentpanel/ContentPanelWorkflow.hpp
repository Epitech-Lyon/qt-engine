/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#pragma once

#include "ContentPanelBase.hpp"
#include "FlowScene.hpp"

#include <QtWidgets/QListWidget>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaMethod>

namespace QtNodes {
	class FlowView;
	class FlowScene;
	class DataModelRegistry;
}

namespace libraryObjects {
	class AObject;
}

namespace qtengine {
	class ListWidget : public QListWidget {
		Q_OBJECT

	public:
		ListWidget(QWidget *parent = nullptr);
		~ListWidget() = default;

	private:
	};

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
		void onAddMethod();
		void onAddSignal();
		void onAddSlot();
		void onAddProperty();

	private:
		typedef struct DataMethod {
		} DataMethod;

		typedef struct DataProperty {
		} DataProperty;

		std::shared_ptr<QtNodes::DataModelRegistry> generateRegistry(const QMetaObject *metaObject, QMetaMethod::Access minimumAccess);

		QtNodes::FlowView *_view;
		FlowScene *_scene;
		std::shared_ptr<QtNodes::DataModelRegistry> _viewRegistry;

		ListWidget *_listMethod;
		QList<DataMethod> _methods;
		ListWidget *_listSignal;
		QList<DataMethod> _signals;
		ListWidget *_listSlot;
		QList<DataMethod> _slots;
		ListWidget *_listProperty;
		QList<DataProperty> _properties;
	};
}
