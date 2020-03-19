/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** FlowSceneWorkflow
*/

#pragma once

#include "FlowScene.hpp"

namespace libraryObjects {
	class AObject;
	class ObjectClass;
}

namespace qtengine {
	class FlowSceneWorkflow : public QtNodes::FlowScene {
		Q_OBJECT

	public:
		FlowSceneWorkflow(QObject *parent = nullptr);
		~FlowSceneWorkflow() = default;

	signals:
		void objectClassDropped(const QPointF &pos, libraryObjects::ObjectClass *objectClass, libraryObjects::AObject *reference, QObject *source);

	private:
		void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
		void dropEvent(QGraphicsSceneDragDropEvent *event) override;
	};
}
