/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewConverter
*/

#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QMap>
#include <functional>

namespace libraryObjects {
	class AObject;

	class ViewConverter {
	public:
		ViewConverter() = default;
		~ViewConverter() = default;

		QJsonObject serialize(AObject *object);
		AObject *deserialize(const QJsonObject &json, bool isRoot = true);

	private:
		QMap<AObject*, std::function<void ()>> _objectData;
		QMap<AObject*, std::function<void ()>> _objectProperties;
	};
}
