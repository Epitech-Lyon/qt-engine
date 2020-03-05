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
		void onAddFunction();
		void onAddSignal();
		void onAddSlot();
		void onAddVariable();

	private:
		typedef struct DataFunction {
		} DataFunction;

		typedef struct DataVariable {
		} DataVariable;

		std::shared_ptr<QtNodes::DataModelRegistry> generateRegistry(const QMetaObject *metaObject, QMetaMethod::Access minimumAccess);

		QtNodes::FlowView *_view;
		FlowScene *_scene;
		std::shared_ptr<QtNodes::DataModelRegistry> _viewRegistry;

		ListWidget *_listFunction;
		QList<DataFunction> _functions;
		ListWidget *_listSignal;
		QList<DataFunction> _signals;
		ListWidget *_listSlot;
		QList<DataFunction> _slots;
		ListWidget *_listVariable;
		QList<DataVariable> _variables;
	};
}
