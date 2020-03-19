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
		Type(const QString &type)
			: _type(type)
		{
		}

		QtNodes::NodeDataType type() const override { return {_type, _type}; }

	private:
		QString _type;
	};
}
