/*
** EPITECH PROJECT, 2019
** qt-engine
** File description:
** NodeData
*/

#pragma once

#include <QtCore/QString>
#include <memory>

namespace QtNodes {
	struct NodeDataType
	{
		QString id;
		QString name;
	};

	/// Class represents data transferred between nodes.
	/// @param type is used for comparing the types
	/// The actual data is stored in subtypes
	class NodeData {
	public:
		virtual ~NodeData() = default;

		virtual bool sameType(std::shared_ptr<NodeData> const &nodeData) const
		{
			return this->type().id == nodeData->type().id;
		}

		/// Type for inner use
		virtual NodeDataType type() const { return {}; }
	};
}
