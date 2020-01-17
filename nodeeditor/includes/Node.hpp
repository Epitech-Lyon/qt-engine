/*
** EPITECH PROJECT, 2019
** forward_coding_journey
** File description:
** Node
*/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QUuid>

#include <QtCore/QJsonObject>

#include "PortType.hpp"

#include "NodeState.hpp"
#include "NodeGeometry.hpp"
#include "NodeData.hpp"
#include "NodeGraphicsObject.hpp"
#include "ConnectionGraphicsObject.hpp"
#include "Serializable.hpp"
#include "memory.hpp"

namespace QtNodes {
	class Connection;
	class ConnectionState;
	class NodeGraphicsObject;
	class NodeDataModel;

	class Node : public QObject, public Serializable {
		Q_OBJECT

	public:
		/// NodeDataModel should be an rvalue and is moved into the Node
		Node(std::unique_ptr<NodeDataModel> && dataModel);
		virtual ~Node();

		QJsonObject save() const override;
		void restore(QJsonObject const &json) override;

		QUuid id() const;
		void reactToPossibleConnection(PortType, NodeDataType const &, QPointF const &scenePoint);
		void resetReactionToConnection();

		NodeGraphicsObject const &nodeGraphicsObject() const;
		NodeGraphicsObject &nodeGraphicsObject();
		void setGraphicsObject(std::unique_ptr<NodeGraphicsObject>&& graphics);
		NodeGeometry &nodeGeometry();
		NodeGeometry const &nodeGeometry() const;
		NodeState const &nodeState() const;
		NodeState &nodeState();
		NodeDataModel *nodeDataModel() const;

	public Q_SLOTS:
		/// Propagates incoming data to the underlying model.
		void propagateData(std::shared_ptr<NodeData> nodeData, PortIndex inPortIndex) const;

		/// Fetches data from model's OUT #index port
		/// and propagates it to the connection
		void onDataUpdated(PortIndex index);

		/// update the graphic part if the size of the embeddedwidget changes
		void onNodeSizeUpdated();

	private:
		QUuid _uid;

		// data
		std::unique_ptr<NodeDataModel> _nodeDataModel;
		NodeState _nodeState;
		// painting
		NodeGeometry _nodeGeometry;
		std::unique_ptr<NodeGraphicsObject> _nodeGraphicsObject;
	};
}
