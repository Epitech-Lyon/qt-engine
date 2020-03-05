/*
** CODING JOURNEY
** workflow
** Type
** LHUILE Léo
*/

#pragma once

#include "NodeData.hpp"
#include <QtCore/QMetaType>

namespace qtengine {
	class Type : public QtNodes::NodeData {
	public:
		Type(int typeId)
			: _typeName(QMetaType::typeName(typeId))
			, _type("QMetaType::Type::" + _typeName)
		{
		}

		QtNodes::NodeDataType type() const override { return {_type, _typeName}; }

	private:
		QString _typeName;
		QString _type;
	};
}
