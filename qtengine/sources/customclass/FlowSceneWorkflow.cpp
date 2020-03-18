/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** FlowSceneWorkflow
*/

#include "moc_FlowSceneWorkflow.cpp"
#include "FlowSceneWorkflow.hpp"

#include "MimeDataObject.hpp"

#include <QtWidgets/QGraphicsSceneDragDropEvent>

qtengine::FlowSceneWorkflow::FlowSceneWorkflow(QObject *parent)
	: QtNodes::FlowScene(parent)
{
}

void qtengine::FlowSceneWorkflow::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	if (dynamic_cast<const MimeDataObject*>(event->mimeData()))
		event->accept();
	else
		event->ignore();
}

void qtengine::FlowSceneWorkflow::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	auto mimeDataObject = dynamic_cast<const MimeDataObject*>(event->mimeData());
	if (!mimeDataObject) { return; }

	emit objectClassDropped(event->scenePos(), mimeDataObject->objectClass(), mimeDataObject->reference());
}
