/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewConverter
*/

#pragma once

#include <QtCore/QJsonObject>

namespace libraryObjects {
	class AObject;

	class ViewConverter {
	public:
		static QJsonObject serialize(AObject *);
		AObject *deserialize(const QJsonObject &);

	private:
		ViewConverter() = default;
		~ViewConverter() = default;
	};
}
