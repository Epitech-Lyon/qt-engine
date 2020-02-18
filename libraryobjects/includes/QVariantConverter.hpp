/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** QVariantConverter
*/

#pragma once

#include <QtCore/QJsonValue>
#include <QtCore/QVariant>

namespace libraryObjects {
	class QVariantConverter {
	public:
		static QJsonValue serialize(const QVariant &variant);
		static QVariant deserialize(const QJsonValue &value);
	};
}
