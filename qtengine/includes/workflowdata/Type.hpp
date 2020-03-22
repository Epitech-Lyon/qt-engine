/*
** CODING JOURNEY
** workflow
** Type
** LHUILE Léo
*/

#pragma once

#include "NodeData.hpp"

#include <QtCore/QMetaType>
#include "LibraryObjectManager.hpp"

namespace qtengine {
	class Type : public QtNodes::NodeData {
	public:
		Type(const QString &type)
			: _type(type)
		{
		}

		bool sameType(std::shared_ptr<NodeData> const &nodeData) const override
		{
			auto first = libraryObjects::LibraryObjectManager::instance()->libraryObjectOfType(this->type().id);
			auto second = libraryObjects::LibraryObjectManager::instance()->libraryObjectOfType(nodeData->type().id);

			if (first && second)
				return first->classHierarchy().startsWith(second->classHierarchy());
			else
				return this->type().id == nodeData->type().id;
		}

		QtNodes::NodeDataType type() const override { return {_type, _type}; }

	private:
		QString _type;
	};
}
